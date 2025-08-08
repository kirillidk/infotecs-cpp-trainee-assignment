#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace test_application {
    template<typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue() : is_running_(true) {}

        ~ThreadSafeQueue() { stop(); }

        template<typename U>
        void push(U &&item) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (not is_running_) {
                return;
            }
            queue_.push(std::forward<U>(item));
            condition_.notify_one();
        }

        void pop(T &item) {
            std::unique_lock<std::mutex> lock(mutex_);

            condition_.wait(lock, [this] { return not queue_.empty() || not is_running_; });

            if (not is_running_ && queue_.empty()) {
                return;
            }

            item = std::move(queue_.front());
            queue_.pop();
        }

        void stop() {
            std::lock_guard<std::mutex> lock(mutex_);
            is_running_ = false;
            condition_.notify_all();
        }

        bool is_running() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return is_running_;
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

    private:
        std::queue<T> queue_;
        mutable std::mutex mutex_;
        std::condition_variable condition_;
        bool is_running_;
    };
} // namespace test_application
