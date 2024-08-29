#pragma once

#include <chrono>
#include <string>

class Timer {
public:
    explicit Timer(std::string &&name = "");
    long long ElapsedMilliseconds() const;

    ~Timer();

private:
    std::string timerName_;
    std::chrono::steady_clock::time_point start_;
};
