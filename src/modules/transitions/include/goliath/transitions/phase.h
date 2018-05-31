#pragma once

#include <chrono>
#include <functional>
#include <vector>

namespace goliath::transitions {
    class Phase {
    public:
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

        Phase(const std::chrono::milliseconds duration, unsigned ticksPerSecond, double min, double max,
              std::function<double(double)> method);

        double get(const unsigned tick) const;

        unsigned getTicksPerSecond() const;
        unsigned getTicks() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        const std::chrono::milliseconds duration;
        unsigned ticksPerSecond;
        double min, max;

        unsigned ticks;

        std::vector<double> preCalculated;
    };
}
