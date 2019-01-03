//
// Created by jskalska on 12/11/2018.
//

#include "Timer.h"

void Timer::start()
{
    startTime = std::chrono::system_clock::now();
}

void Timer::stop()
{
    endTime = std::chrono::system_clock::now();
}

int64_t Timer::getTimeInSeconds()
{
    return std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
}

int64_t Timer::getTimeInMilliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

int64_t Timer::getTimeWithoutStopping() {
    typedef std::chrono::duration<float> float_seconds;
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime).count();
}
