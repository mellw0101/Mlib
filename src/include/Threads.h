#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "def.h"

namespace Mlib::Threads
{
    template <typename ThreadPoolType, typename Func, typename... Args>
    auto
    enqueueTask(ThreadPoolType &pool, Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        return pool.enqueue(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    class ThreadPool
    {
    public:
        ThreadPool(u64 threads);
        ~ThreadPool();

        template <class Callback, class... Args>
        auto
        enqueue(Callback &&f, Args &&...args) -> std::future<typename std::invoke_result<Callback, Args...>::type>
        {
            using return_type = typename std::invoke_result<Callback, Args...>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                bind(std::forward<Callback>(f), std::forward<Args>(args)...));

            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (stop)
                {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }

                tasks.emplace(
                    [task]()
                    {
                        (*task)();
                    });
            }
            condition.notify_one();

            return res;
        }

    private:
        std::vector<std::thread>          workers;
        std::queue<std::function<void()>> tasks;
        std::mutex                        queueMutex;
        std::condition_variable           condition;
        bool                              stop;
    };

    template <typename... ParamTypes>
    class iter_thread_t
    {
    public:
        // Constructor: Takes the interval in milliseconds, the task to perform, and parameters for the task
        iter_thread_t(unsigned interval, std::function<void(ParamTypes...)> task, ParamTypes... params)
            : interval_(interval)
            , task_(move(task))
            , active_(true)
            , params_(make_tuple(std::forward<ParamTypes>(params)...))
        {
            worker_ = std::thread(
                [this]()
                {
                    this->loop();
                });
        }

        ~iter_thread_t()
        {
            active_.store(false);
            if (worker_.joinable())
            {
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

        void
        loop()
        {
            using namespace std::chrono;
            auto next_run_time = steady_clock::now() + milliseconds(interval_);

            while (active_.load())
            {
                auto now = steady_clock::now();
                if (now >= next_run_time)
                {
                    apply(task_, params_);                         // Use std::apply to pass tuple elements as arguments
                    next_run_time = now + milliseconds(interval_); // Schedule the next run
                }
                else
                {
                    std::this_thread::sleep_for(milliseconds(1)); // Sleep for a short while to prevent busy waiting
                }
            }
        }
    };
} // namespace Mlib::Threads

template <typename ThreadPoolType, typename Func, typename... Args>
auto
enqueueTask(ThreadPoolType &pool, Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
{
    return pool.enqueue(std::forward<Func>(func), std::forward<Args>(args)...);
}


#define enqueueT(__Name, __Pool, ...) std::future<void> __Name = __Pool.enqueue(__VA_ARGS__)
