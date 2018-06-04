#include <goliath/transitions/phase.h>
#include <goliath/transitions/util/transition_utilities.h>

#include <cmath>

using namespace goliath::transitions;

Phase::Phase(const std::chrono::milliseconds duration, const size_t ticksPerSecond, const double min,
             const double max, std::function<double(double)> method)
    : duration(duration), ticksPerSecond(ticksPerSecond), min(min), max(max),
      ticks(static_cast<Tick>(std::ceil(duration.count() / (1.0 / ticksPerSecond * 1000)))) {

    for (Tick tick = 1; tick <= ticks; ++tick) {
        double value = util::TransitionUtilities::map(static_cast<double>(tick), static_cast<double>(ticks),
                                                      static_cast<double>(1));
        preCalculated.push_back(goliath::util::TransitionUtilities::map(method(value), 0.0, 1.0, min, max));
    }
}

double Phase::get(const Tick tick) const {
    return util::TransitionUtilities::map(preCalculated[tick], 0.0, 1.0, min, max);
}

const std::chrono::milliseconds Phase::getDuration() const {
    return duration;
}

size_t Phase::getTicksPerSecond() const {
    return ticksPerSecond;
}

size_t Phase::getNumberOfTicks() const {
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
