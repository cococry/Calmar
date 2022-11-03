#include "pch.h"
#include "random.hpp"

namespace calmar {
    random::random(uint_least32_t seed) {
        this->_randomNumberGenerator = std::mt19937(seed);
        this->_realDistribution = std::uniform_real_distribution<double_t>();
        this->_bytedistribution = std::uniform_int_distribution<int32_t>(0, 256);
    }

    random::~random() {
    }
    i32 random::nextInt() {
        return this->nextInt(0, INT32_MAX);
    }
    i32 random::nextInt(int32_t maxValue) {
        return this->nextInt(0, maxValue);
    }
    i32 random::nextInt(int32_t minValue, int32_t maxValue) {
        std::uniform_int_distribution<int32_t> distribution(minValue, maxValue);
        return distribution(this->_randomNumberGenerator);
    }

    double random::nextDouble() {
        return this->_realDistribution(this->_randomNumberGenerator);
    }
    double random::nextDouble(double_t minValue, double_t maxValue) {
        std::uniform_real_distribution<double_t> distribution(minValue, minValue);
        return this->_realDistribution(this->_randomNumberGenerator);
    }
}  // namespace calmar
