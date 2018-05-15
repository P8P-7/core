#pragma once

/**
 * @file vision_utilities.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::vision_utilities
     * @brief Contains useful utility function for vision calculations
     */
    class vision_utilities {
    public:
        /**
         * @brief Maps a numeric value in a range to a the same position in another range
         * @tparam T Numeric value
         * @param input Input value
         * @param input_min Input range lower bound
         * @param input_max Input range upper bound
         * @param output_min Output range lower bound
         * @param output_max Output range upper bound
         * @return Numeric value mapped to new range
         */
        template<typename T>
        static T map(T input, T input_min, T input_max, T output_min, T output_max) {
            return (input - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
        }

        template<typename T>
        static T map(T input, T input_max, T output_max) {
            return map(input, static_cast<T>(0), input_max, static_cast<T>(0), output_max);
        }

    };
}
