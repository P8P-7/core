#include <goliath/transitions/reel.h>
#include <goliath/transitions/util/transition_utilities.h>
#include <goliath/transitions/util/transition_error.h>

using namespace goliath::transitions;

Reel::Reel(const size_t ticksPerSecond)
        : Tickable(ticksPerSecond), ticks(0), currentTick(0), callback(nullptr) {
}

Reel::Reel(const size_t ticksPerSecond, std::function<void(double)> callback)
        : Tickable(ticksPerSecond), ticks(0), currentTick(0),
          callback(std::make_unique<std::function<void(double)>>(callback)) {
}


void Reel::addPhase(const std::chrono::milliseconds duration, double min, double max,
                    std::function<double(double)> method) {
    auto phase = std::make_shared<Phase>(duration, ticksPerSecond, min, max, method);
    addPhase(phase);
}

void Reel::addPhase(const std::shared_ptr<Phase> phase) {
    if (phase->getTicksPerSecond() != getTicksPerSecond()) {
        throw goliath::exceptions::TransitionError("Phase tps and reel tps do not match");
    }

    phases.push_back(phase);
    ticks += phase->getNumberOfTicks();
    duration += phase->getDuration();

    for (double value : *phase) {
        preCalculated.push_back(value);
    }
}

size_t Reel::getTicks() const {
    return ticks;
}

double Reel::getTick() {
    if (currentTick > ticks) {
        throw goliath::exceptions::TransitionError("Tick out of range");
    }

    return preCalculated[currentTick];
}

void Reel::tick() {
    if (callback != nullptr) {
        (*callback)(getTick());
    }

    currentTick++;
}

bool Reel::canTick() const {
    return currentTick < ticks;
}

Reel::iterator Reel::begin() {
    return preCalculated.begin();
}

Reel::iterator Reel::end() {
    return preCalculated.end();
}

Reel::const_iterator Reel::begin() const {
    return preCalculated.cbegin();
}

Reel::const_iterator Reel::end() const {
    return preCalculated.cend();
}
