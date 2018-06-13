#pragma once

#include <memory>

#include "phase.h"
#include "tickable.h"

/**
 * @file reel.h
 * @author Group 7 - Informatica
 */

namespace goliath::transitions {
    /**
     * @class goliath::transitions::Reel
     * @brief Collection of phases that can be one after another
     */
    class Reel : public Tickable {
    public:
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        explicit Reel(size_t ticksPerSecond);
        /**
         * @param callback Update on every tick
         */
        Reel(size_t ticksPerSecond, std::function<void(double)> callback);

        /**
         * @brief Add a phase to this reel by creating a new one

         */
        void addPhase(std::chrono::milliseconds duration, double min, double max,
                      std::function<double(double)> method);

        /**
         * @brief Add a predefined phase to this reel
         * @param phase The new phase to be added
         */
        void addPhase(std::shared_ptr<Phase> phase);

        /**
         * @brief Get the value at the current tick
         * @return Value at current tick
         */
        double getTick();
        void tick() override;
        /**
         * @brief Checks whether the reel is not out of bounds
         * @return True if not out of bounds
         */
        bool canTick() const;

        /**
         * @brief Get the amount of ticks in this reel
         * @return Amount of ticks
         */
        size_t getNumberOfTicks() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::vector<std::shared_ptr<Phase>> phases;
        std::vector<double> preCalculated;

        size_t ticks;

        Tick currentTick;

        std::unique_ptr<std::function<void(double)>> callback;
    };
}
