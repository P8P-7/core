#pragma once

#include <chrono>

/**
 * @file tickable.h
 * @author Group 7 - Informatica
 */

namespace goliath::transitions {
    /**
     * @class goliath::transitions::Tickable
     * @brief Interface for defining whether a this transition can be executed or not
     */
    class Tickable {
    public:
        /**
         * @param ticksPerSecond Speed of the transition
         */
        Tickable(const unsigned ticksPerSecond);

        /**
         * @brief Get the next tick value
         */
        virtual void tick() = 0;
        /**
         * @return Length of this transition
         */
        virtual std::chrono::milliseconds getDuration() const;
        /**
         * @return Get the speed of this transition
         */
        virtual const unsigned getTicksPerSecond() const;


    protected:
        mutable std::chrono::milliseconds duration;
        const unsigned ticksPerSecond;

    };
}
