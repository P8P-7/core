#include <goliath/transitions/phase.h>
#include <goliath/transitions/util/transition_utilities.h>

using namespace goliath::transitions;

Phase::Phase(const std::chrono::milliseconds duration, unsigned ticksPerSecond, double min, double max,
             std::function<double(double)> method)
        : duration(duration), ticksPerSecond(ticksPerSecond), min(min), max(max) {
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    ticks = static_cast<unsigned>(milliseconds.count() /  (1.0 / ticksPerSecond * 1000.0));

    for (unsigned tick = 0; tick < ticks; ++tick) {
        double value = util::TransitionUtilities::map(static_cast<double>(tick), static_cast<double>(ticks),
                                                      static_cast<double>(1));
        preCalculated.push_back(goliath::util::TransitionUtilities::map(method(value), 0.0, 1.0, min, max));
    }
}

double Phase::get(const unsigned tick) const {
    return util::TransitionUtilities::map(preCalculated[tick], 0.0, 1.0, min, max);
}

unsigned Phase::getTicksPerSecond() const {
    return ticksPerSecond;
}

unsigned Phase::getTicks() const {
    return ticks;
}

Phase::iterator Phase::begin() {
    return preCalculated.begin();
}

Phase::iterator Phase::end() {
    return preCalculated.end();
}

Phase::const_iterator Phase::begin() const {
    return preCalculated.cbegin();
}

Phase::const_iterator Phase::end() const {
    return preCalculated.cend();
}
