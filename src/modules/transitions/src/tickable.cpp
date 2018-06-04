#include <goliath/transitions/tickable.h>

using namespace goliath::transitions;

Tickable::Tickable(const size_t ticksPerSecond)
        : duration(0), ticksPerSecond(ticksPerSecond) {}

std::chrono::milliseconds Tickable::getDuration() const {
    return duration;
}

const size_t Tickable::getTicksPerSecond() const {
    return ticksPerSecond;
}
