/**
 * @brief Logic for generating random numbers via the Mersenne Twister Engine or by typical standard, seeded random
 * number generator in order to produce a random bool
 * @authors Brad Peterson Ph.D., John Z. DeGrey, et al.
 * @copyright Weber State University
 */

#include <iostream>
#include <random>

#ifndef ALGOGAUGE_RANDOMNUM_HPP
#define ALGOGAUGE_RANDOMNUM_HPP

/**
 * Initial class for holding all the Random Number generators
 * @tparam T defaults to unsigned 32 bit int. May use something different if preferred
 */
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

/**
 * Only constructor for this class
 * @tparam T defaults to unsigned 32 bit int. May use something different if preferred
 * @param min The lower bound number to calculate the random number for each iteration
 * @param max The upper bound number to calculate the random number for each iteration
 */
template <typename T>
RandomNum<T>::RandomNum(T min, T max) {
    std::random_device newRandomDevice; //Will be used to obtain a seed for the random number engine
    mersenneTwisterEngine = std::mt19937(newRandomDevice()); //Standard mersenne_twister_engine seeded with rd()
    uniIntDistribution = std::uniform_int_distribution<T>(min, max);
    //Clion raises a warning for this, however, we're not looking at generating a random sequence of numbers, just
    //a random bool, making this less predictable that what Clion claims.
    std::srand(time(0));
}

/**
 * Generates a new random number via the Mersenne Twister Engine
 * @tparam T defaults to unsigned 32 bit int. May use something different if preferred
 * @return T object within the bounds set in the constructor
 */
template <typename T>
T RandomNum<T>::getRandomNumber() {
    return uniIntDistribution(mersenneTwisterEngine);
}

/**
 * Generates a bool value of true or false at random with each iteration. Does NOT use the Mersenne Twister Engine
 * @tparam T defaults to unsigned 32 bit int. May use something different if preferred
 * @return Either true or false at random
 */
template <typename T>
bool RandomNum<T>::randomBool() {
    //Clion raises a warning for this, however, we're not looking at generating a random sequence of numbers, just
    //a random bool, making this less predictable that what Clion claims.
    return bool(std::rand() % 2);
}

#endif //ALGOGAUGE_RANDOMNUM_HPP
