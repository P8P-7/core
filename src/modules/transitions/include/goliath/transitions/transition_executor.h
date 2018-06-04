#pragma once

#include <memory>

#include "tickable.h"

/**
 * @file transition_executor.h
 * @author Group 7 - Informatica
 */

namespace goliath::transitions {
    /**
     * @class goliath::transitions::TransitionExecutor
     * @brief Executes a transition
     */
    class TransitionExecutor {
    public:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Milliseconds = std::chrono::milliseconds;
        using Microseconds = std::chrono::microseconds;

        /**
         * @brief Executes a class implementing the tickable interface by taking their tps and duration and calling the
         * tick method within the specified duration
         * @param tickable Tickable to be run
         * @return Amount of skipped ticks
         */
        static size_t execute(std::shared_ptr<Tickable> tickable);

    };
}
