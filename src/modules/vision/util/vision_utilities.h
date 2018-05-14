#pragma once

namespace goliath::vision {
    class vision_utilities {
    public:

        template<typename T>
        static T map(T input, T input_min, T input_max, T output_min, T output_max) {
            return (input - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
        }

        template<typename T>
        static T map(T input, T input_max, T output_max) {
            return map(input, 0, input_max, 0, output_max);
        }

    };
}
