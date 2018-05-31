#pragma once

#include <memory>

#include "phase.h"

namespace goliath::transitions {
    class Reel {
    public:
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        Reel(const unsigned ticksPerSecond);

        void addPhase(const std::chrono::milliseconds duration, double min, double max,
                      std::function<double(double)> method);

        void addPhase(const std::shared_ptr<Phase> phase);

        double tick();
        void tick(std::function<void(double)> callback);

        unsigned getTicksPerSecond() const;
        unsigned getTicks() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::vector<std::shared_ptr<Phase>> phases;
        std::vector<double> preCalculated;

        const unsigned ticksPerSecond;
        unsigned ticks;

        unsigned currentTick;
    };
}
