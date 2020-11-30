//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "spip.h"
#include <poll.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

struct cancellable_file {
  BOOL init;
  int fd;
  int cancelfd[2];
};

typedef struct cancellable_file cancellable_file_t;

void
cancellable_file_destroy(cancellable_file_t *self)
{
  if (self->init) {
    close(self->fd);
    close(self->cancelfd[0]);
    close(self->cancelfd[1]);
  }

  free(self);
}

BOOL
cancellable_file_cancel(cancellable_file_t *self)
{
  char b = 0;
  int ret;

  ret = write(self->cancelfd[1], &b, 1);

  return ret == 1;
}

int
cancellable_file_read(
    const cancellable_file_t *self,
    void *data,
    size_t size)
{
  struct pollfd pollfd[2];
  int ret;

  pollfd[0].fd     = self->fd;
  pollfd[0].events = POLLIN;
  pollfd[0].revents = 0;

  pollfd[1].fd     = self->cancelfd[0];
  pollfd[1].events = POLLIN;
  pollfd[1].revents = 0;

  fflush(stdout);

  if (poll(pollfd, 2, -1) == -1)
    return -1;

  if (pollfd[0].revents == POLLNVAL || pollfd[1].revents == POLLNVAL)
    return -1;

  if (pollfd[1].revents == POLLIN)
    return 0;

  ret = read(self->fd, data, size);

  return ret;
}

int
cancellable_file_write(cancellable_file_t *self, const void *data, size_t size)
{
  return write(self->fd, data, size);
}

cancellable_file_t *
cancellable_file_new(int fd)
{
  cancellable_file_t *inst = NULL;

  ALLOCATE(inst, cancellable_file_t);

  TRYC(pipe(inst->cancelfd));
  inst->fd = fd;
  inst->init = TRUE;

  return inst;

fail:
  if (inst != NULL)
    cancellable_file_destroy(inst);

  return NULL;
}

static BOOL
spip_file_close(void *user)
{
  cancellable_file_t *file = (cancellable_file_t *) user;

  cancellable_file_cancel(file);

  cancellable_file_destroy(file);

  return FALSE;
}

static BOOL
spip_file_write(void *user, const void *data, size_t size)
{
  cancellable_file_t *file = (cancellable_file_t *) user;

  return (size_t) cancellable_file_write(file, data, size) == size;
}

static BOOL
spip_file_read_byte(void *user, uint8_t *c)
{
  cancellable_file_t *file = (cancellable_file_t *) user;

  return cancellable_file_read(file, c, 1) == 1;
}

BOOL
spip_interface_open_serial(
    spip_iface_t *iface,
    const char *path,
    unsigned int baud)
{
  struct termios2 tio;
  struct stat sbuf;
  cancellable_file_t *file = NULL;
  int fd = -1;
  BOOL ok = FALSE;

  if ((fd = open(path, O_RDWR)) == -1) {
    fprintf(
        stderr,
        "Failed to open `%s' in RW mode: %s\n",
        path,
        strerror(errno));
    goto fail;
  }

  if (stat(path, &sbuf) != -1 && S_ISCHR(sbuf.st_mode)) {
    TRYC(ioctl(fd, TCGETS2, &tio));

    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = baud;
    tio.c_ospeed = baud;

    TRYC(ioctl(fd, TCSETS2, &tio));
  }

  CONSTRUCT(file, cancellable_file, fd);


  ok = TRUE;

fail:
  if (ok) {
    iface->write      = spip_file_write;
    iface->read_byte  = spip_file_read_byte;
    iface->userdata   = file;
    iface->close      = spip_file_close;
  } else if (fd != -1) {
    close(fd);
  }
  return ok;
}

