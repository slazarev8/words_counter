#include "Timer.h"

#include <iostream>

Timer::Timer(std::string &&name) : timerName_(std::move(name)) {
    start_ = std::chrono::steady_clock::now();
    std::cout << timerName_ << ": Timer is started\n";
}

long long Timer::ElapsedMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start_)
            .count();
}

Timer::~Timer() {
    std::cout << timerName_ << ": " << ElapsedMilliseconds() << " milliseconds\n";
}