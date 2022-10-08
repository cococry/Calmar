#pragma once

#include "calmar/core/defines.hpp"

#include <random>

namespace calmar {
    class CALMAR_API random {
       public:
        random(uint_least32_t seed);
        random() : random(std::_Random_device()){};
        ~random();
        i32 nextInt();
        i32 nextInt(int32_t maxValue);
        i32 nextInt(int32_t minValue, int32_t maxValue);
        double nextDouble();
        double nextDouble(double_t minValue, double_t maxValue);

       private:
        std::mt19937 _randomNumberGenerator;
        std::uniform_real_distribution<double_t> _realDistribution;
        std::uniform_int_distribution<int32_t> _bytedistribution;
    };
}  // namespace calmar
