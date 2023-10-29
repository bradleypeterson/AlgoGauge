/**
 * @brief TODO
 * @authors John Z. DeGrey, Brad Peterson Ph.D., et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <random>

#ifndef ALGOGAUGE_RANDOMNUM_HPP
#define ALGOGAUGE_RANDOMNUM_HPP

template <typename T = uint32_t>
class RandomNum {
public:
    //constructors and destructors
    explicit RandomNum(T min = 0, T max = UINT32_MAX);
    ~RandomNum() = default;

    //class members
    T getRandomNumber();
    static bool randomBool();

private:
    std::mt19937 mersenneTwisterEngine;
    std::uniform_int_distribution<T> uniIntDistribution;
};

template <typename T>
RandomNum<T>::RandomNum(T min, T max) {
    std::random_device newRandomDevice; //Will be used to obtain a seed for the random number engine
    mersenneTwisterEngine = std::mt19937(newRandomDevice()); //Standard mersenne_twister_engine seeded with rd()
    uniIntDistribution = std::uniform_int_distribution<T>(min, max);
    std::srand(time(0));
}

template <typename T>
T RandomNum<T>::getRandomNumber() {
    return uniIntDistribution(mersenneTwisterEngine);
}

template <typename T>
bool RandomNum<T>::randomBool() {
    return bool(std::rand() % 2);
}

#endif //ALGOGAUGE_RANDOMNUM_HPP
