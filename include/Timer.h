//
// Created by jskalska on 12/11/2018.
//

#pragma once
#include <iostream>
#include <chrono>

class Timer
{
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
public:
    void start();
    void stop();
    int64_t getTimeInSeconds();
    int64_t getTimeInMilliseconds();
    int64_t getTimeWithoutStopping();
};
