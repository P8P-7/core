#pragma once

/**
 * @file transition_utilities.h
 * @author Group 7 - Informatica
 */

namespace goliath::util {
    /**
     * @class goliath::util::TransitionUtilities
     * @brief Contains utilities for transitions
     */
    class TransitionUtilities {
    public:
        template<typename T>
        static T map(T input, T inputMin, T inputMax, T outputMin, T outputMax) {
            return (input - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
        }

        template<typename T>
        static T map(T input, T inputMax, T outputMax) {
            return map(input, static_cast<T>(0), inputMax, static_cast<T>(0), outputMax);
        }

    };
}
