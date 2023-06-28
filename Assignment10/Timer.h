#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer() : start(clock::now()) {}

    template <typename T>
    static T toMilliSec(T nanosec) {
        return T(nanosec / 1e6);
    }

    template <typename T>
    static T toMicroSec(T nanosec) {
        return T(nanosec / 1e3);
    }

    // Returns the elapsed time in nanoseconds since the Timer was created
    int64_t getElapsedNanoSec() const {
        const clock::time_point now = clock::now();
        return std::chrono::duration_cast<nanosec>(now - start).count();
    }

private:
    using clock = std::chrono::steady_clock;
    using nanosec = std::chrono::nanoseconds;

    clock::time_point start;  // The time point when the Timer was created
};

#endif
