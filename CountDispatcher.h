#pragma once

#include "WordsCounter.h"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

class TTasksDispatcher {
public:
    explicit TTasksDispatcher(size_t threadsCnt, size_t maxTasksSize) : maxTasksSize_(maxTasksSize) {
        for (size_t i = 0; i < threadsCnt; i++) {
            threads_.emplace_back(&TTasksDispatcher::DoJob, this);
        }
    }

    void DoJob() {
        while (true) {
            std::unique_lock ul(jobMutex_);
            tasksCv_.wait(ul, [&]() { return !tasks_.empty() || finish_; });
            if (!tasks_.empty()) {
                auto jobToDo = tasks_.back();
                tasks_.pop_back();
                ul.unlock();
                jobToDo->CountWords();
                maxJobsCv_.notify_one();
            } else if (finish_)
                return;
        }
    }

    void AddJob(std::shared_ptr<TWordsCounter> newJob) {
        std::unique_lock lg(jobMutex_);
        if (tasks_.size() >= maxTasksSize_)
            maxJobsCv_.wait(lg, [&]() { return tasks_.size() < maxTasksSize_; });
        tasks_.push_front(std::move(newJob));
        tasksCv_.notify_one();
    }

    void Join() {
        finish_ = true;
        tasksCv_.notify_all();
        for (auto &thread: threads_) {
            thread.join();
        }
    }

private:
    std::deque<std::shared_ptr<TWordsCounter>> tasks_;
    std::vector<std::thread> threads_;
    std::condition_variable tasksCv_;
    std::mutex jobMutex_;
    std::condition_variable maxJobsCv_;
    std::atomic<bool> finish_ = false;
    const size_t maxTasksSize_;
};