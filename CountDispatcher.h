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
    explicit TTasksDispatcher(size_t threadsCnt, size_t maxTasksSize);

    void DoJob();
    void AddJob(std::shared_ptr<TWordsCounter> newJob) ;
    void Join();

private:
    std::deque<std::shared_ptr<TWordsCounter>> tasks_;
    std::vector<std::thread> threads_;
    std::condition_variable tasksCv_;
    std::mutex jobMutex_;
    std::condition_variable maxJobsCv_;
    std::atomic<bool> finish_ = false;
    const size_t maxTasksSize_;
};