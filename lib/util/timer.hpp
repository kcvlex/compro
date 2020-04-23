#pragma once
#include "template.cpp"
#include <chrono>

struct Timer {
    const std::chrono::system_clock::time_point start;

    Timer() : start(std::chrono::system_clock::now()) { }

    auto elapsed() const {
        auto cur = std::chrono::system_clock::now();
        return cur - start;
    }

    // msec
    double diff() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed()).count();
    }
};
