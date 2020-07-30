/*
 * defs.h: Some generic macros
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef _DEFS_H
#define _DEFS_H

#include <pthread.h>
#include "util.h"

#define PACKED __attribute__((packed))

#define TRYC(expr) TRY((expr) != -1)

#define CASSERT(predicate) _impl_CASSERT_LINE(predicate,__LINE__, CURRFILE)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#ifndef CURRFILE
#  define CURRFILE DEFS_H
#endif /* CURRFILE */

#define MT_SAFE       \
  struct {                   \
    pthread_mutex_t mutex;    \
    BOOL mutex_init;          \
    BOOL acquired;            \
  }

#define MT_SHARED      \
  struct {                  \
    MT_SAFE;           \
    unsigned int refcnt;    \
    void (*_dtor) (void *); \
  }

#define MT_WAKEABLE    \
  struct {                  \
    MT_SAFE;           \
    BOOL cond_init;         \
    pthread_cond_t cond;    \
  }

#define MT_INIT(self)      \
    {                           \
      pthread_mutexattr_t attr; \
      pthread_mutexattr_init(&attr); \
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
      TRY(pthread_mutex_init(&self->mutex, &attr) != -1); \
      self->mutex_init = TRUE;  \
    }

#define MT_SHARED_INIT(self, type)             \
    {                                               \
      MT_INIT(self);                           \
      self->refcnt = 1;                             \
      self->_dtor = (void *) JOIN(type, _destroy);  \
    }

#define MT_WAKEABLE_INIT(self)                    \
    {                                                  \
      MT_INIT(self);                              \
      TRY(pthread_cond_init(&self->cond, NULL) != -1); \
      self->cond_init = TRUE;                          \
    }

#define MT_FINALIZE(self)  \
    if (self->mutex_init)       \
      pthread_mutex_destroy(&self->mutex);

#define MT_WAKEABLE_FINALIZE(self)  \
{                                        \
  if (self->cond_init)                   \
    pthread_cond_destroy(&self->cond);   \
  MT_FINALIZE(self);                \
}

#define MT_WAIT(self) (pthread_cond_wait(&self->cond, &self->mutex) != -1)
#define MT_SIGNAL(self) (pthread_cond_signal(&self->cond) != -1)

#define MT_LOCK_NOCHECK(self) pthread_mutex_lock(&self->mutex)
#define MT_UNLOCK_NOCHECK(self) pthread_mutex_unlock(&self->mutex)

#define MT_LOCK_INTERNAL(self)                                    \
    (self->acquired = pthread_mutex_lock(&self->mutex) != -1, \
    self->acquired)

#define MT_UNLOCK_INTERNAL(self)                                         \
  (!(self->acquired = pthread_mutex_unlock(&self->mutex) == -1))

#define MT_LOCK(self)             \
    BOOL JOIN(self, _acquired) = FALSE; \
    TRY(MT_LOCK_INTERNAL(self));   \
    JOIN(self, _acquired) = TRUE;

#define MT_UNLOCK(self)               \
    if (JOIN(self, _acquired)) {            \
      (void) MT_UNLOCK_INTERNAL(self); \
    }

#define MT_REF(self)   \
{                           \
  TRY(MT_LOCK_INTERNAL(self));  \
  ++(self)->refcnt;         \
  TRY(MT_UNLOCK_INTERNAL(self));\
}

#define MT_UNREF(self)      \
{                                \
  (void) MT_LOCK_INTERNAL(self);     \
  if (--(self)->refcnt > 0) {    \
    (void) MT_UNLOCK_INTERNAL(self); \
  } else {                       \
    (void) MT_UNLOCK_INTERNAL(self); \
    ((self)->_dtor) (self);      \
  }                              \
}

#ifndef logfunc
#  define logfunc(severity, fmt, arg...)           \
    fprintf(stderr, "[%-8s] " fmt, STRINGIFY(severity), ##arg)
#endif /* logfunc */

#define log_critical(fmt, arg...) logfunc(CRITICAL, fmt, ##arg)
#define log_error(fmt, arg...)    logfunc(ERROR, fmt, ##arg)
#define log_warning(fmt, arg...)  logfunc(WARNING, fmt, ##arg)
#define log_info(fmt, arg...)     logfunc(INFO, fmt, ##arg)
#define log_debug(fmt, arg...)    logfunc(DEBUG, fmt, ##arg)

#define ENSURE(expr, code) if (!(expr)) { code; }
#define ASSERT(expr, action)                    \
    ENSURE(                                     \
        expr,                                   \
        log_critical(                            \
            "ASSERTION FAILED: `%s'\n",         \
            STRINGIFY(expr));                   \
            action                              \
    )

#define TRYCATCH(expr, action)                  \
    ENSURE(                                     \
        expr,                                   \
        log_error(                               \
            "exception in expression `%s'\n",   \
            STRINGIFY(expr));                   \
            action                              \
    )

#define TRY(expr) TRYCATCH(expr, goto fail)

#define ALLOCATE_MANY(dest, n, type)            \
    TRYCATCH(                                   \
        dest = calloc(n, sizeof(type)),         \
        log_error(                               \
            "failed to allocate %d objects of type %s\n", \
            n,                                  \
            STRINGIFY(type)                     \
            ); goto fail)

#define ALLOCATE(dest, type) ALLOCATE_MANY(dest, 1, type)

#define INIT(self, type, ...)            \
  TRYCATCH(                                     \
      JOIN(type, _init) (self, __VA_ARGS__),    \
      log_error(                                 \
          "failed to call constructor of class \"%s\"\n", \
          STRINGIFY(type)); goto fail)         \


#define CONSTRUCT(dest, type, ...)        \
  TRYCATCH(                                     \
      dest = JOIN(type, _new) (__VA_ARGS__),    \
      log_error(                                 \
          "failed to construct object of class \"%s\"\n", \
          STRINGIFY(type)); goto fail)


enum boolean {
  FALSE,
  TRUE
};

typedef enum boolean BOOL;

#endif /* _DEFS_H */
