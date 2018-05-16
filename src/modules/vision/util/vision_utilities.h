#pragma once

/**
 * @file vision_utilities.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::VisionUtilities
     * @brief Contains useful utility function for vision calculations
     */
    class VisionUtilities {
    public:
        /**
         * @brief Maps a numeric value in a range to a the same position in another range
         * @tparam T Numeric value
         * @param input Input value
         * @param inputMin Input range lower bound
         * @param inputMax Input range upper bound
         * @param outputMin Output range lower bound
         * @param outputMax Output range upper bound
         * @return Numeric value mapped to new range
         */
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
