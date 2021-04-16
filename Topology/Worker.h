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

#ifndef _TOPOLOGY_WORKER_H
#define _TOPOLOGY_WORKER_H

#include <list>
#include <string>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>

#define PH_NS_PER_SEC 1000000000ll
#define PH_NS_PER_MS  (PH_NS_PER_SEC / 1000ll)
#define PH_WAIT_MS_DEFAULT 3600000ll

namespace PLCTool {
  class Worker;

  class WorkerTask {
  public:
    enum Type {
      ONESHOT,
      LOOPING
    };

  protected:
      Worker *worker = nullptr;
      void restart(void);

  private:
    Type type = ONESHOT;
    uint64_t intervalMs = 0;
    uint64_t lastSpawnMs = 0;
    bool running = false;
    friend class Worker;

  public:
    WorkerTask(Type type = ONESHOT, uint64_t intervalMs = 0);
    virtual ~WorkerTask(void);

    virtual void run(void) = 0;
    virtual void cleanup(void) = 0;

    bool
    attached(void) const
    {
      return this->worker != nullptr;
    }

    void
    setInterval(uint16_t ms)
    {
      this->intervalMs = ms;
    }

    bool attach(Worker *);
    void detach(void);
    void cancel(void); // Convenience method, just calls worker->remove()
  };

  class Cancellable {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    bool mutexInitialized = false;
    bool condInitialized  = false;
    bool waiting = false;

  public:
    void wait(uint64_t ms);
    void cancel(void);

    Cancellable(void);
    ~Cancellable();
  };

  typedef std::list<WorkerTask *> WorkerTaskQueue;
  class Worker {
    std::string name;
    Cancellable cancellable;
    WorkerTaskQueue taskQueue;
    pthread_mutex_t taskQueueMutex;
    bool taskQueueMutexInitialized = false;

    pthread_t taskQueueThread;
    bool taskQueueThreadRunning = false;

    bool halting = false;

    void work(uint64_t &waitMs);
    static void *workerThread(void *);

    void lock(void);
    void unlock(void);
    void wakeUp(void);
    void removeUnsafe(WorkerTask *);
    void removeUnsafe(WorkerTaskQueue::iterator);

  public:
    void pushFromWorker(WorkerTask *);
    void push(WorkerTask *);
    void remove(WorkerTask *);

    Worker(std::string const &name = "NO-NAME");
    ~Worker();
  };
}

#endif // _TOPOLOGY_SWITCH_H

