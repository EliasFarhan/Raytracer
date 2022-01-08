#include "random_generator.h"

namespace ray
{
    Random* Random::instance_ = nullptr;
    Random& Random::GetInstance()
    {
        return *instance_;
    }
}
