#pragma once

#include <chrono>
#include <functional>
#include <vector>

/**
 * @file phase.h
 * @author Group 7 - Informatica
 */

namespace goliath::transitions {
    /**
     * @class goliath::transitions::Phase
     * @brief A piece of a reel with form
     */
    class Phase {
    public:
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        /**
         * @param duration Length of this phase
         * @param ticksPerSecond Speed of this phase
         * @param min Lower bound of the phase (mapped to 0)
         * @param max Upper bound of the phase (mapped to 1)
         * @param method Type of transition (e. g. linear, eases)
         */
        Phase(const std::chrono::milliseconds duration, const unsigned ticksPerSecond, const double min,
              const double max, std::function<double(double)> method);

        /**
         * @brief Get value at specific tick
         * @param tick Tick
         * @return Value at tick
         */
        double get(const unsigned tick) const;
        /**
         * @brief Get length of this phase
         * @return Duration
         */
        const std::chrono::milliseconds getDuration() const;
        /**
         * @brief Get speed of this phase
         * @return Ticks per Second
         */
        unsigned getTicksPerSecond() const;
        /**
         * @brief Get total amount of ticks this phase uses
         * @return Total ticks
         */
        unsigned getTicks() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        const std::chrono::milliseconds duration;
        const unsigned ticksPerSecond;
        const double min, max;

        const unsigned ticks;

        std::vector<double> preCalculated;
    };
}
