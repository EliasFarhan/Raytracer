#pragma once

#include <chrono>

namespace ray
{
    using seconds = std::chrono::duration<double>;
    class Timer
    {
    public:
        Timer();

        seconds Restart();

        [[nodiscard]] seconds GetElapsedTime() const;
    private:
        std::chrono::high_resolution_clock clock_;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    };
}