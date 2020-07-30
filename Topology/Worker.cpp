//
// Worker.cpp: Asynchronous task queue
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include <Topology/Worker.h>
#include <Topology/Exception.h>
#include <algorithm>

using namespace PLCTool;

void
Cancellable::wait(uint64_t ms)
{
  struct timespec timeToWait;

  if (ms > 0) {
    PH_CHECK_GENERIC(
        clock_gettime(CLOCK_REALTIME, &timeToWait) != -1,
        "Cannot get real time");

    timeToWait.tv_nsec += ms * PH_NS_PER_MS;

    if (timeToWait.tv_nsec > PH_NS_PER_SEC) {
      uint64_t seconds   = timeToWait.tv_nsec / PH_NS_PER_SEC;
      timeToWait.tv_sec  += seconds;
      timeToWait.tv_nsec -= seconds * PH_NS_PER_SEC;
    }

    PH_CHECK_GENERIC(
        pthread_mutex_lock(&this->mutex) != -1,
        "Failed to acquire cond mutex");

    this->waiting = true;

    (void) pthread_cond_timedwait(
            &this->cond,
            &this->mutex,
            &timeToWait);

    this->waiting = false;

    PH_CHECK_GENERIC(
        pthread_mutex_unlock(&this->mutex) != -1,
        "Failed to release mutex");
  } else {
    PH_CHECK_GENERIC(
        pthread_mutex_lock(&this->mutex) != -1,
        "Failed to acquire cond mutex");

    this->waiting = true;

    PH_CHECK_GENERIC(
        pthread_cond_wait(&this->cond, &this->mutex) != -1,
        "Failed to wait for condition");

    this->waiting = false;

    PH_CHECK_GENERIC(
        pthread_mutex_unlock(&this->mutex) != -1,
        "Failed to release mutex");
  }
}

void
Cancellable::cancel(void)
{
  PH_CHECK_GENERIC(
      pthread_mutex_lock(&this->mutex) != -1,
      "Failed to acquire cond mutex");

  PH_CHECK_GENERIC(
      pthread_cond_signal(&this->cond) != -1,
      "Failed to signal cancellable");

  PH_CHECK_GENERIC(
      pthread_mutex_unlock(&this->mutex) != -1,
      "Failed to release mutex");
}

Cancellable::Cancellable()
{
  PH_CHECK_GENERIC(
      pthread_mutex_init(&this->mutex, nullptr) != -1,
      "Failed to create cond mutex");
  this->mutexInitialized = true;

  PH_CHECK_GENERIC(
      pthread_cond_init(&this->cond, nullptr) != -1,
      "Failed to initialize condition variable");
  this->condInitialized = true;
}

Cancellable::~Cancellable()
{
  if (this->waiting)
    this->cancel();

  if (this->condInitialized)
    pthread_cond_destroy(&this->cond);

  if (this->mutexInitialized)
    pthread_mutex_destroy(&this->mutex);
}


//////////////////////////// WorkerTask object ////////////////////////////////
WorkerTask::WorkerTask(Type type, uint64_t intervalMs)
{
  this->type = type;
  this->intervalMs = intervalMs;
}

bool
WorkerTask::attach(Worker *wk)
{
  PH_ASSERT(wk != nullptr);

  if (this->worker != nullptr)
    return false;

  this->worker = wk;

  return true;
}

void
WorkerTask::detach(void)
{
  this->worker = nullptr;
}

void
WorkerTask::cancel(void)
{
  if (this->worker != nullptr)
    this->worker->remove(this);
}

void
WorkerTask::restart(void)
{
  this->worker->pushFromWorker(this);
}

WorkerTask::~WorkerTask()
{
  if (this->worker != nullptr)
    this->cancel();
}

////////////////////////////// Worker object //////////////////////////////////
void
Worker::removeUnsafe(WorkerTask *task)
{
  this->removeUnsafe(
      std::find(
          this->taskQueue.begin(),
          this->taskQueue.end(),
          task));
}

void
Worker::removeUnsafe(WorkerTaskQueue::iterator p)
{
  WorkerTask *task = nullptr;

  if (!this->halting) {
    PH_ASSERT(p != this->taskQueue.end());

    task = *p;

    PH_ASSERT(!task->running);
    PH_ASSERT(task != nullptr);
    PH_ASSERT(task->worker != nullptr);
    PH_ASSERT(task->worker == this);

    task->detach();

    task->cleanup();

    this->taskQueue.erase(p);
  }
}

static uint64_t
getCurrentTimeMs(void)
{
  struct timespec now;
  int64_t nowMs;

  PH_CHECK_GENERIC(
        clock_gettime(CLOCK_REALTIME, &now) != -1,
        "Cannot get real time");
  nowMs = now.tv_sec * 1000 + now.tv_nsec / PH_NS_PER_MS;

  return nowMs;
}

void
Worker::work(uint64_t &waitMs)
{
  WorkerTask *task;
  uint64_t nowMs = getCurrentTimeMs();
  int64_t taskWaitMs;
  int64_t elapsedMs;

  waitMs = PH_WAIT_MS_DEFAULT;

  this->lock();

  auto p = this->taskQueue.begin();

  while (p != this->taskQueue.end()) {
    task = *p;
    task->running = true;

    switch (task->type) {
      case WorkerTask::Type::ONESHOT:
        this->unlock();
        task->run();
        this->lock();
        task->running = false;
        this->removeUnsafe(p++);
        break;

      case WorkerTask::Type::LOOPING:
        if (task->lastSpawnMs == 0)
          task->lastSpawnMs = getCurrentTimeMs();
        elapsedMs = nowMs - task->lastSpawnMs;
        taskWaitMs = task->intervalMs - elapsedMs;

        // Run all pending invocations
        while (taskWaitMs <= 0) {
          task->lastSpawnMs += task->intervalMs;

          this->unlock();
          task->run();
          this->lock();
          taskWaitMs = task->lastSpawnMs + task->intervalMs - getCurrentTimeMs();
        }

        if (taskWaitMs < static_cast<int64_t>(waitMs))
          waitMs = taskWaitMs;
        ++p;
        break;
    }
    task->running = false;
  }

  this->unlock();

}

void
Worker::wakeUp(void)
{
  this->cancellable.cancel();
}

void *
Worker::workerThread(void *ptr)
{
  Worker *self = static_cast<Worker *>(ptr);
  uint64_t waitMs;

  while (!self->halting) {
    self->work(waitMs);
    if (waitMs > 0)
      self->cancellable.wait(waitMs);
  }

  return nullptr;
}

void
Worker::lock(void)
{
  PH_CHECK_GENERIC(
      pthread_mutex_lock(&this->taskQueueMutex) != -1,
      "Failed to acquire worker mutex");
}

void
Worker::unlock(void)
{
  PH_CHECK_GENERIC(
      pthread_mutex_unlock(&this->taskQueueMutex) != -1,
      "Failed to release worker mutex");
}

void
Worker::pushFromWorker(WorkerTask *task)
{
  PH_ASSERT(task->worker == nullptr);
  task->worker = this;
  this->taskQueue.push_back(task);

  // No need to wakeUp
}

void
Worker::push(WorkerTask *task)
{
  PH_ASSERT(task->worker == nullptr);

  task->worker = this;

  this->lock();
  this->taskQueue.push_back(task);
  this->unlock();
  this->wakeUp();
}

void
Worker::remove(WorkerTask *worker)
{
  this->lock();
  this->removeUnsafe(worker);
  this->unlock();

  this->wakeUp();
}

Worker::~Worker()
{
  this->halting = true;

  if (this->taskQueueThreadRunning) {
    this->wakeUp();
    pthread_join(this->taskQueueThread, nullptr);
  }
  if (this->taskQueueMutexInitialized)
    pthread_mutex_destroy(&this->taskQueueMutex);

  for (auto p : this->taskQueue)
    p->cleanup();
}

Worker::Worker(std::string const &name)
{
  this->name = name;

  PH_CHECK_GENERIC(
      pthread_mutex_init(&this->taskQueueMutex, nullptr) != -1,
      "Failed to initialize Worker mutex");
  this->taskQueueMutexInitialized = true;

  PH_CHECK_GENERIC(
      pthread_create(
          &this->taskQueueThread,
          nullptr,
          Worker::workerThread,
          this) != -1,
      "Failed to initialize worker thread");
  this->taskQueueThreadRunning = true;
}

