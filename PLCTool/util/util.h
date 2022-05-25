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

#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef linux
# include <linux/unistd.h>
# define TID_FUNC (int) syscall (224)
#else
# define TID_FUNC (int) getpid ()
#endif

#define RECOMMENDED_LINE_SIZE 256

#ifndef MIN
#  define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#  define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef SIGN
#  define SIGN(x) (!(x < 0) - !(x > 0))
#endif

#define _JOIN(a, b) a ## b
#define JOIN(a, b) _JOIN(a, b)

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY (x)

#define C_ASSERT(name, expr) \
  typedef char JOIN(JOIN(assert_, name), _failed)[2*!!(expr)-1]

#define IN_BOUNDS(x, range) (((x) >= 0) && ((x) < (range)))

#define WARNING(fmt, arg...)                                        \
      do {                                                          \
        errno_save ();                                              \
        fprintf (stderr, "(%d) warning (%s@" __FILE__ ":%d): " fmt, \
          TID_FUNC,                                                 \
          __FUNCTION__, __LINE__, ## arg);                          \
        errno_restore ();                                           \
      } while (0)

#define ERROR(fmt, arg...)                                          \
      do {                                                          \
        errno_save ();                                              \
        fprintf (stderr, "(%d) error (%s@" __FILE__ ":%d): " fmt,   \
          TID_FUNC,                                                 \
          __FUNCTION__, __LINE__, ## arg);                          \
        errno_restore ();                                           \
      } while (0)

#define NOTICE(fmt, arg...)                                         \
      do {                                                          \
        errno_save ();                                              \
        fprintf (stderr, "(%d) notice (%s@" __FILE__ ":%d): " fmt,  \
          TID_FUNC,                                                 \
          __FUNCTION__, __LINE__, ## arg);                          \
        errno_restore ();                                           \
      } while (0)


#ifndef NDEBUG
  #define DEBUG(fmt, arg...)                                        \
      do {                                                          \
        errno_save ();                                              \
        fprintf (stderr, "(%d) debug: " __FILE__ ":%d: %s: " fmt,   \
          TID_FUNC,                                                 \
          __LINE__, __FUNCTION__, ## arg);                          \
        errno_restore ();                                           \
      } while (0)bJHbBmqWL@2m&&

#else
  #define DEBUG(fmt, arg...)
#endif

#define PTR_LIST(type, name)                         \
  type ** name ## _list;                             \
  int     name ## _count;

#define PTR_LIST_LOCAL(type, name)                   \
  type ** name ## _list = NULL;                      \
  int     name ## _count = 0;

#define PTR_LIST_EXTERN(type, name)                  \
  extern type ** name ## _list;                      \
  extern int     name ## _count;

#define PTR_LIST_INIT(where, name)                   \
  where->name ## _list = NULL;                       \
  where->name ## _count = 0;                 

#define PTR_LIST_APPEND(name, ptr)                   \
  ptr_list_append ((void ***) &JOIN (name, _list),   \
                   &JOIN (name, _count), ptr)

#define PTR_LIST_APPEND_CHECK(name, ptr)                   \
  ptr_list_append_check ((void ***) &JOIN (name, _list),   \
                   &JOIN (name, _count), ptr)

#define PTR_LIST_REMOVE(name, ptr)  \
  ptr_list_remove_first ((void ***) &JOIN (name, _list),   \
                   &JOIN (name, _count), ptr)

#define FOR_EACH_PTR(this, where, name)              \
  int JOIN (_idx_, __LINE__);                             \
  for (JOIN (_idx_, __LINE__) = 0;                        \
       JOIN (_idx_, __LINE__) < where->name ## _count;    \
       JOIN (_idx_, __LINE__)++)                          \
    if ((this = where->name ## _list[                \
      JOIN (_idx_, __LINE__)]) != NULL)

# define __UNITS(x, wrdsiz) ((((x) + (wrdsiz - 1)) / wrdsiz))
# define __ALIGN(x, wrdsiz) (__UNITS(x, wrdsiz) * wrdsiz)

struct growbuf
{
  union {
    void *base;
    uint8_t *as_bytes;
  };

  size_t size;
  size_t allocation;
};

#define growbuf_INITIALIZER {{NULL}, 0, 0}

struct strlist
{
  PTR_LIST (char, strings);
};

typedef struct _al
{
  int    al_argc;
  char **al_argv;

  char  *al_line;
}
arg_list_t;

void  al_append_argument (arg_list_t *, const char*);
void  free_al (arg_list_t *);

arg_list_t *csv_split_line (const char *);
arg_list_t *split_line (const char *);

void *xmalloc (size_t siz);
void *xrealloc (void *p, size_t siz);
char *xstrdup (const char *s);
int   is_asciiz (const char *buf, int lbound, int ubound);
char *vstrbuild (const char *fmt, va_list ap);
char *strbuild (const char *fmt, ...);
char *str_append_char (char* source, char c);
char *fread_line (FILE *fp);
void ptr_list_append (void ***, int *, void *);
int  ptr_list_append_check (void ***, int *, void *);
int  ptr_list_remove_first (void ***, int *, void *);
int  ptr_list_remove_all (void ***, int *, void *);

void errno_save (void);
void errno_restore (void);

struct strlist *strlist_new (void);
void strlist_append_string (struct strlist *, const char *);
void strlist_walk (struct strlist *, void *, void (*) (const char *, void *));
void strlist_destroy (struct strlist *);
void strlist_debug (const struct strlist *);
void strlist_cat (struct strlist *, const struct strlist *);
void strlist_union (struct strlist *, const struct strlist *);
int  strlist_have_element (const struct strlist *, const char *);
unsigned int yday_to_daymonth (int, int);

char *trim (const char *);
char *rtrim (const char *);
char *ltrim (const char *);
int lscanf (const char *, ...);
int lscanf_huge (const char *, ...);

void print_hexdump(const u_char *bytes, uint length);
char *make_hexdump(
    const u_char *bytes,
    uint length,
    uint word_size,
    uint words_per_line,
    bool tabulate);

#ifdef __sun__ /* puto Solaris */
int dprintf (int fd, const char *fmt, ...);
#endif

void growbuf_init(struct growbuf *buf);
void growbuf_reset(struct growbuf *buf);
void *growbuf_alloc(struct growbuf *buf, size_t size);
off_t growbuf_get_offset(const struct growbuf *buf, const void *addr);
void growbuf_finalize(struct growbuf *buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UTIL_H */

