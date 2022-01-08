#pragma once
#include <random>

namespace ray
{
    class Random
    {
        public:
            Random() : e2(rd())
            {
                instance_ = this;
            }
        template <typename T>
                requires std::is_floating_point_v<T>
            T Range(T min, T max)
            {
                std::uniform_real_distribution<T> dist(min, max);
                return dist(e2);
            }

            template <typename T>
                requires std::is_integral_v<T>
            T Range(T min, T max)
            {
                std::uniform_int_distribution<T> dist(min, max);
                return dist(e2);
            }

            static Random& GetInstance();

        private:
            static Random* instance_;
            std::random_device rd;
            std::mt19937 e2;
    };
    
} // namespace ray
