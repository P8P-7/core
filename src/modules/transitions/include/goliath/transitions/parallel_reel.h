#pragma once

#include <memory>

#include "tickable.h"
#include "reel.h"

/**
 * @file parallel_reel.h
 * @author Group 7 - Informatica
 */

namespace goliath::transitions {
    /**
     * @class goliath::transitions::ParallelReel
     * @brief Runs multiple reels at the same time
     */
    class ParallelReel : public Tickable {
    public:
        ParallelReel(std::vector<std::shared_ptr<Reel>> reels);

        void tick() override;

        /**
         * @return Duration of longest reel
         */
        std::chrono::milliseconds getDuration() const override;
        /**
         * @return Speed of all reels
         */
        const size_t getTicksPerSecond() const override;

    private:
        const std::vector<std::shared_ptr<Reel>> reels;
    };
}
