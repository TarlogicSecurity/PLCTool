//
// Worker.h: Asynchronous task queue
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
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

