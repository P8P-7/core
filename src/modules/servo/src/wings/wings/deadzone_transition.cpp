#include <cmath>
#include <numeric>
#include "deadzone_transition.h"

using namespace goliath;
using namespace goliath::servo;

DeadzoneTransitionController::DeadzoneTransitionController(WingController *controller,
                                                           handles::WingHandle &handle,
                                                           Direction direction,
                                                           std::uint16_t speed)
        : history(historySize),
          started(false),
          controller(controller),
          handle(handle),
          direction(direction),
          speed(speed),
          state(controller->wingStateRepository->getState(handle.getId())) {}


DeadzoneTransitionController::DeadzoneTransitionController(WingController *controller, WingCommand command)
        : DeadzoneTransitionController(controller, command.handle,
                                       command.handle.getTranslatedDirection(command.direction), command.speed) {}

void DeadzoneTransitionController::prestart() {
    controller->_setSpeed(handle, speed);
    controller->_setAngle(handle, getBoundaryPosition(), false);
}

bool DeadzoneTransitionController::canStart() {
    return !handle.getDevice()->isMoving();
}

void DeadzoneTransitionController::start() {
    controller->_setMode(handle, servo::ServoMode::WHEEL);
    controller->_setSpeed(handle, speed, direction);

    started = true;
}

void DeadzoneTransitionController::stop() {
    controller->_setSpeed(handle, 0);
    controller->_setMode(handle, servo::ServoMode::JOINT);
    controller->_setSpeed(handle, speed);

    auto sector = state.getNextSector(direction);
    BOOST_LOG_TRIVIAL(trace) << "Handle " << handle.getId() << " has transitioned to sector " << sector;
    controller->wingStateRepository->updateWorldAngle(handle.getId(),
                                                      controller->getAngleFromServo(handle),
                                                      sector
    );

    started = false;
}

bool DeadzoneTransitionController::isStarted() const {
    return started;
}

bool DeadzoneTransitionController::isInDeadzone() const {
    if (!started) {
        return false;
    }

    if (history.size() < historySize) {
        //BOOST_LOG_TRIVIAL(trace) << "Buffer too small";
        return true;
    }

    if ((direction == servo::Direction::CLOCKWISE && history.front() >= history.back()) ||
        (direction == servo::Direction::COUNTER_CLOCKWISE && history.front() <= history.back())) {
        //BOOST_LOG_TRIVIAL(trace) << "Slope direction is valid";

        int sum = std::accumulate(history.begin(), history.end(), 0);
        if (sum == 0) {
            //BOOST_LOG_TRIVIAL(trace) << "Buffer contains only zeros";
            return true;
        }

        int target = getTargetPosition();
        sum = std::accumulate(history.begin(), history.begin() + historySize, 0);
        auto average = static_cast<int>(sum / historySize);
        if (std::abs(static_cast<long>(target - average)) > margin) {
            //BOOST_LOG_TRIVIAL(trace) << "Average position is too high "
            //                         << "average: " << std::to_string(average)
            //                         << ", target: " << std::to_string(target);
            return true;
        }
        //BOOST_LOG_TRIVIAL(trace) << "Average position is in range " << std::to_string(average);

        return false;
    }

    return true;
}

bool DeadzoneTransitionController::tick() {
    if (!started) {
        throw std::runtime_error("Transition needs to be started before ticking");
    }

    auto device = handle.getDevice();
    int currentPosition = device->getPresentPosition();
    //BOOST_LOG_TRIVIAL(trace) << "Position: " << currentPosition;
    if (currentPosition < 0 || currentPosition > 1023) {
        //BOOST_LOG_TRIVIAL(trace) << "Position is out of range";
        return true;
    }

    if (!history.empty() && currentPosition == history.back()) {
        //BOOST_LOG_TRIVIAL(trace) << "Value is the same as the previous";
        return true;
    }

    if (!history.empty() && !isInThreshold(currentPosition)) {
        //BOOST_LOG_TRIVIAL(trace) << "Value is outside threshold";
        return true;
    }

    history.emplace_back(currentPosition);
    if (history.size() > bufferSize) {
        history.pop_front();
    }

    return isInDeadzone();
}

int DeadzoneTransitionController::getThreshold() const {
    return direction == servo::Direction::CLOCKWISE ? 1024 - threshold : threshold;
}

bool DeadzoneTransitionController::isInThreshold(int value) const {
    return value > 0 &&
           ((direction == servo::Direction::CLOCKWISE && value > getThreshold()) || value < getThreshold());
}

int DeadzoneTransitionController::getTargetPosition() const {
    return direction == servo::Direction::CLOCKWISE ? 1023 : 0;
}

int DeadzoneTransitionController::getBoundaryPosition() const {
    return direction == servo::Direction::CLOCKWISE ? 0 : 300;
}