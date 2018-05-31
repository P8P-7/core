#include <goliath/transitions/reel.h>
#include <goliath/transitions/util/transition_utilities.h>
#include <goliath/transitions/util/transition_error.h>

using namespace goliath::transitions;

Reel::Reel(const unsigned ticksPerSecond)
        : ticksPerSecond(ticksPerSecond), ticks(0), currentTick(0) {
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
    ticks += phase->getTicks();

    for (double value : *phase) {
        preCalculated.push_back(value);
    }
}

unsigned Reel::getTicksPerSecond() const {
    return ticksPerSecond;
}

unsigned Reel::getTicks() const {
    return ticks;
}

double Reel::tick() {
    currentTick++;
    return preCalculated[currentTick];
}

void Reel::tick(std::function<void(double)> callback) {
    callback(tick());
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
