#include <goliath/transitions/tickable.h>

using namespace goliath::transitions;

Tickable::Tickable(const unsigned ticksPerSecond)
        : duration(0), ticksPerSecond(ticksPerSecond) {}

std::chrono::milliseconds Tickable::getDuration() const {
    return duration;
}

const unsigned Tickable::getTicksPerSecond() const {
    return ticksPerSecond;
}
