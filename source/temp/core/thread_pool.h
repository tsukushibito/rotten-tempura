#pragma once

#include <cstdint>

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "temp/core/logger.h"

namespace temp {
namespace core {

class ThreadPool {
 public:
  explicit ThreadPool(size_t worker_count);
  ~ThreadPool();

  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = default;

  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type>;

  void waitForTasks();

 private:
  std::vector<std::thread> workers_;
  std::vector<std::thread::id> thread_ids_;
  std::vector<std::uint8_t> thread_runnings_;

  std::queue<std::function<void()> > tasks_;

  std::mutex queue_mutex_;
  std::condition_variable condition_;

  bool stop_;
};

inline ThreadPool::ThreadPool(size_t worker_count) : stop_(false) {
  thread_ids_.resize(worker_count);
  thread_runnings_.resize(worker_count);

  for (size_t i = 0; i < worker_count; ++i)
    workers_.emplace_back([this, i] {
      thread_runnings_[i] = 1;
      thread_ids_[i] = std::this_thread::get_id();
      for (;;) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);

          condition_.wait(lock, [this, i] {
            auto awaked = stop_ || !tasks_.empty();
            thread_runnings_[i] = awaked ? 1 : 0;
            return awaked;
          });

          if (stop_ && tasks_.empty()) {
            thread_runnings_[i] = 0;
            return;
          }

          task = std::move(tasks_.front());
          tasks_.pop();
        }
        task();
      }
    });
}

inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread& worker : workers_) {
    worker.join();
  }
}

template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()> >(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    if (stop_) {
      TEMP_LOG_INFO("enqueue on stopped ThreadPool");
      return std::future<return_type>();
    }

    auto func = [t = std::move(task)]() { (*t)(); };
    tasks_.emplace(func);
  }
  condition_.notify_one();
  return res;
}

void ThreadPool::waitForTasks() {
  while (std::any_of(thread_runnings_.begin(), thread_runnings_.end(),
                     [](std::uint8_t runnings) { return runnings == 1; })) {
  }
}

}  // namespace core
}  // namespace temp
