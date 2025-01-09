#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Vector.h"
#include "def.h"

namespace Mlib::Threads {
  template <typename ThreadPoolType, typename Func, typename... Args>
  auto enqueueTask(ThreadPoolType &pool, Func &&func, Args &&...args) -> FUTURE<decltype(func(args...))> {
    return pool.enqueue(FORWARD<Func>(func), FORWARD<Args>(args)...);
  }

  class ThreadPool {
   public:
    ThreadPool(u64 threads);
    ~ThreadPool();

    template <class Callback, class... Args>
    auto enqueue(Callback &&f, Args &&...args) -> FUTURE<typename INVOKE_RESULT<Callback, Args...>::type> {
      using namespace std;
      using return_type = typename invoke_result<Callback, Args...>::type;
      auto task         = make_shared<packaged_task<return_type()>>(bind(forward<Callback>(f), forward<Args>(args)...));
      future<return_type> res = task->get_future();
      {
        unique_lock<mutex> lock(queueMutex);
        if (stop) {
          throw runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task]() {
          (*task)();
        });
      }
      condition.notify_one();
      return res;
    }

   private:
    VECTOR<THREAD>               workers;
    QUEUE<std::function<void()>> tasks;
    MUTEX                        queueMutex;
    CONDITION_VARIABLE           condition;
    bool                         stop;
  };

  template <typename... ParamTypes>
  class iter_thread_t {
   public:
    /* Constructor:  Takes the interval in milliseconds, the task to perform, and parameters for the task */
    iter_thread_t(unsigned interval, std::function<void(ParamTypes...)> task, ParamTypes... params)
        : interval_(interval)
        , task_(move(task))
        , active_(true)
        , params_(make_tuple(std::forward<ParamTypes>(params)...)) {
      worker_ = std::thread([this]() {
        this->loop();
      });
    }

    ~iter_thread_t() {
      active_.store(false);
      if (worker_.joinable()) {
        worker_.join();
      }
    }

    iter_thread_t(const iter_thread_t &)            = delete;
    iter_thread_t &operator=(const iter_thread_t &) = delete;
    iter_thread_t(iter_thread_t &&)                 = delete;
    iter_thread_t &operator=(iter_thread_t &&)      = delete;

   private:
    std::thread       worker_;
    std::atomic<bool> active_;

    // The interval between task executions, in milliseconds
    unsigned interval_;

    // The task to be performed, adjusted as needed
    std::function<void()> task_;

    // Parameters to be used with the task
    std::tuple<ParamTypes...> params_;

    void loop() {
      using namespace std::chrono;
      auto next_run_time = steady_clock::now() + milliseconds(interval_);

      while (active_.load()) {
        auto now = steady_clock::now();
        if (now >= next_run_time) {
          apply(task_, params_);                         // Use std::apply to pass tuple elements as arguments
          next_run_time = now + milliseconds(interval_); // Schedule the next run
        }
        else {
          std::this_thread::sleep_for(milliseconds(1));  // Sleep for a short while to prevent busy waiting
        }
      }
    }
  };  
}

/* Struct that reprecents a future. */
typedef struct MFuture MFuture;

void MFuture_destroy(MFuture *future);
void *MFuture_get(MFuture *future);
MFuture *MFuture_submit(void *(*task)(void *), void *arg);