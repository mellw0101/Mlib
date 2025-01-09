#include "../include/Threads.h"

namespace Mlib::Threads {
  ThreadPool ::ThreadPool(Ulong threads) : stop(false) {
    for (Ulong i = 0; i < threads; ++i) {
      workers.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] {
              return stop || !tasks.empty();
            });
            if (stop && tasks.empty()) {
              return;
            }
            task = std::move(tasks.front());
            tasks.pop();
          }
          task();
        }
      });
    }
  }

  ThreadPool ::~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
      worker.join();
    }
  }
}

/* Internal structs with external linkage. */
typedef struct MFuture {
  pthread_mutex_t mutex;
  pthread_cond_t  cond;
  bool  is_ready;
  void *result;
} MFuture;

/* Internal structs. */
typedef struct {
  MFuture *future;
  void *(*task)(void *);
  void *arg;
} MFutureTask;

static MFuture *MFuture_create(void) {
  MFuture *future = (MFuture *)malloc(sizeof(*future));
  if (!future) {
    exit(1);
  }
  pthread_mutex_init(&future->mutex, NULL);
  pthread_cond_init(&future->cond, NULL);
  future->is_ready = FALSE;
  future->result   = NULL;
  return future;
}

void MFuture_destroy(MFuture *future) {
  pthread_mutex_destroy(&future->mutex);
  pthread_cond_destroy(&future->cond);
  free(future);
}

void *MFuture_get(MFuture *future) {
  pthread_mutex_lock(&future->mutex);
  while (!future->is_ready) {
    pthread_cond_wait(&future->cond, &future->mutex);
  }
  void *result = future->result;
  pthread_mutex_unlock(&future->mutex);
  return result;
}

static void *MFuture_thread_func(void *arg) {
  MFutureTask *data = (MFutureTask *)arg;
  void *result = data->task(data->arg);
  pthread_mutex_lock(&data->future->mutex);
  data->future->result   = result;
  data->future->is_ready = TRUE;
  pthread_cond_signal(&data->future->cond);
  pthread_mutex_unlock(&data->future->mutex);
  free(data);
  return NULL;
}

MFuture *MFuture_submit(void *(*task)(void *), void *arg) {
  MFutureTask *data = (MFutureTask *)malloc(sizeof(*data));
  data->future = MFuture_create();
  data->task   = task;
  data->arg    = arg;
  pthread_t thread;
  pthread_create(&thread, NULL, MFuture_thread_func, data);
  pthread_detach(thread);
  return data->future;
}