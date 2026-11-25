// Timer.h - simple stopwatch
#pragma once

#include <chrono>
using namespace std;

class Timer {
private:
    chrono::steady_clock::time_point m_Start;

public:
    Timer() : m_Start(chrono::steady_clock::now()) {}

    void Reset() { m_Start = chrono::steady_clock::now(); }

    long long ElapsedMS() const {
        return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - m_Start)
            .count();
    }

    long long ElapsedSec() const { return ElapsedMS() / 1000; }
};
