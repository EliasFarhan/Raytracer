#include "timer.h"

namespace ray
{
Timer::Timer()
{
    start_ = clock_.now();
}

seconds Timer::Restart()
{
    const auto end = clock_.now();
    const auto dt = end - start_;
    start_ = end;

    return std::chrono::duration_cast<seconds>(dt);
}

seconds Timer::GetElapsedTime() const
{
    const auto end = clock_.now();
    const auto dt = end - start_;
    return std::chrono::duration_cast<seconds>(dt);
}
} // namespace ray
