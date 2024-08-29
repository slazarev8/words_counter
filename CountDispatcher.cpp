#include "CountDispatcher.h"

TTasksDispatcher::TTasksDispatcher(size_t threadsCnt, size_t maxTasksSize)
    : maxTasksSize_(maxTasksSize) {
  for (size_t i = 0; i < threadsCnt; i++) {
    threads_.emplace_back(&TTasksDispatcher::DoJob, this);
  }
}

void TTasksDispatcher::DoJob() {
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

void TTasksDispatcher::AddJob(std::shared_ptr<TWordsCounter> newJob) {
  std::unique_lock lg(jobMutex_);
  if (tasks_.size() >= maxTasksSize_)
    maxJobsCv_.wait(lg, [&]() { return tasks_.size() < maxTasksSize_; });
  tasks_.push_front(std::move(newJob));
  tasksCv_.notify_one();
}

void TTasksDispatcher::Join() {
  finish_ = true;
  tasksCv_.notify_all();
  for (auto &thread : threads_) {
    thread.join();
  }
}
