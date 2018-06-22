#include <utility>

#include <goliath/servo/wings/wing_controller.h>
#include <numeric>
#include "deadzone_transition.h"

using namespace goliath;
using namespace goliath::servo;

WingController::WingController(std::shared_ptr<repositories::WingStateRepository> wingStateRepository)
        : wingStateRepository(std::move(wingStateRepository)) {
}

void WingController::setWorldAngle(handles::WingHandle &handle, double angle, std::uint16_t speed, Direction direction) {
    if (angle < 0 || angle > 360) {
        throw std::out_of_range("Angle out of bounds [0, 360]");
    }

    direction = handle.getTranslatedDirection(direction);

    auto &state = wingStateRepository->getState(handle.getId());
    if (state.getMode() != ServoMode::JOINT) {
        throw std::runtime_error("Servo must be in joint mode to set an angle");
    }

    double sectorAngle = handle.getTranslatedAngle(state.getAngleFromWorldAngle(handle.getTranslatedWorldAngle(angle)));
    if (sectorAngle < 0 || sectorAngle > 300) {
        throw std::out_of_range("Angle is in deadzone");
    }
    auto sector = handle.getTranslatedSector(state.getSectorFromWorldAngle(angle));
    if (sector < 0 || sector >= state.getNumberOfSectors()) {
        throw std::out_of_range("Sector out of bounds");
    }

    BOOST_LOG_TRIVIAL(trace) << "From " << std::to_string(state.getWorldAngle()) << " to " << std::to_string(angle);
    BOOST_LOG_TRIVIAL(trace) << "From angle " << std::to_string(state.getAngle()) << " to "
                             << std::to_string(sectorAngle);
    BOOST_LOG_TRIVIAL(trace) << "From sector " << std::to_string(state.getSector()) << " to " << std::to_string(sector);
    BOOST_LOG_TRIVIAL(trace) << "Direction "
                             << (direction == servo::Direction::CLOCKWISE ? "CW" : "CCW");

    DeadzoneTransitionController transitionController(this, handle, direction, speed);
    while (sector != state.getSector()) {
        transitionController.prestart();
        while (!transitionController.canStart()) ;
        transitionController.start();
        while (transitionController.tick()) ;
        transitionController.stop();
    }
    _setSpeed(handle, speed);
    _setAngle(handle, sectorAngle);

    wingStateRepository->updateWorldAngle(handle.getId(), sectorAngle, sector);
}

void WingController::_setAngle(handles::WingHandle &handle, double angle, bool waitForCompletion) {
    if (angle < 0.0 || angle > 300.0) {
        throw std::out_of_range("Angle out of bounds [0, 300]");
    }

    auto &state = wingStateRepository->getState(handle.getId());
    if (state.getMode() != ServoMode::JOINT) {
        throw std::runtime_error("Servo must be in joint mode to set a pure angle");
    }

    auto device = handle.getDevice();
    auto goal = static_cast<short>(std::floor(angle / 300.0 * 1023));
    BOOST_LOG_TRIVIAL(trace) << "Traversing from angle " << std::to_string(state.getAngle()) << " to " << std::to_string(angle) << " (" << std::to_string(goal) << ")";
    device->setGoalPosition(goal);

    while (waitForCompletion && device->isMoving());
}

void WingController::_setMode(handles::WingHandle &handle, ServoMode mode) {
    auto device = handle.getDevice();

    switch (mode) {
        case ServoMode::JOINT:
            device->setCWAngleLimit(0);
            device->setCCWAngleLimit(0x3FF);
            break;
        case ServoMode::WHEEL:
            device->setCWAngleLimit(0);
            device->setCCWAngleLimit(0);
            break;
    }
}

void WingController::_setSpeed(handles::WingHandle &handle, std::uint16_t speed, servo::Direction direction) {
    if (speed > 1023) {
        throw std::out_of_range("Speed must be in [0, 1024)");
    }
    auto device = handle.getDevice();

    if (direction == servo::Direction::CLOCKWISE) {
        speed += 1024;
    }
    device->setMovingSpeed(speed);
}

void WingController::execute(std::vector<servo::WingCommand> commands) {
    for (auto &command : commands) {
        auto &state = wingStateRepository->getState(command.handle.getId());
        auto direction = command.handle.getTranslatedDirection(command.direction);
        auto targetWorldAngle = command.handle.getTranslatedWorldAngle(command.angle);
        auto targetAngle = command.handle.getTranslatedAngle(state.getAngleFromWorldAngle(command.angle));
        auto targetSector = command.handle.getTranslatedSector(state.getSectorFromWorldAngle(command.angle));

        BOOST_LOG_TRIVIAL(trace) << "From " << std::to_string(state.getWorldAngle()) << " to "
                                 << std::to_string(targetWorldAngle);
        BOOST_LOG_TRIVIAL(trace) << "From angle " << std::to_string(state.getAngle()) << " to "
                                 << std::to_string(targetAngle);
        BOOST_LOG_TRIVIAL(trace) << "From sector " << std::to_string(state.getSector()) << " to "
                                 << std::to_string(targetSector);
        BOOST_LOG_TRIVIAL(trace) << "Direction "
                                 << (direction == servo::Direction::CLOCKWISE ? "CW" : "CCW");
    }

    while (true) {
        BOOST_LOG_TRIVIAL(trace) << "Transitioning sectors";
        std::vector<std::shared_ptr<DeadzoneTransitionController>> controllers{};
        for (auto &command : commands) {
            auto &state = wingStateRepository->getState(command.handle.getId());
            auto targetSector = command.handle.getTranslatedSector(state.getSectorFromWorldAngle(command.angle));

            if (targetSector == state.getSector()) {
                BOOST_LOG_TRIVIAL(trace) << "Handle " << command.handle.getId() << " has reached sector (" << targetSector << ")";
                continue;
            } else {
                BOOST_LOG_TRIVIAL(trace) << "Moving " << command.handle.getId() << " from sector " << state.getSector() << " to " << targetSector;
            }

            auto controller = std::make_shared<DeadzoneTransitionController>(this, command);
            controller->prestart();
            controllers.push_back(controller);
        }
        if (controllers.empty()) {
            break;
        }
        BOOST_LOG_TRIVIAL(trace) << "Started controllers " << controllers.size();

        while (true) {
            bool start = true;
            for (auto &controller : controllers) {
                if (!controller->canStart()) {
                    start = false;
                    break;
                }
            }
            if (start) {
                break;
            }
        }

        for (auto &controller : controllers) {
            controller->start();
        }

        BOOST_LOG_TRIVIAL(trace) << "Transitioning";
        while (true) {
            size_t count = 0;
            for (auto &controller : controllers) {
                if (!controller->isStarted()) {
                    count++;
                } else if (!controller->tick()) {
                    controller->stop();
                }
            }
            if (count == controllers.size()) {
                break;
            }
        }
    }

    for (auto &command : commands) {
        auto &state = wingStateRepository->getState(command.handle.getId());
        auto targetAngle = command.handle.getTranslatedAngle(state.getAngleFromWorldAngle(command.angle));

        _setSpeed(command.handle, command.speed);
        _setAngle(command.handle, targetAngle, false);
        wingStateRepository->updateWorldAngle(command.handle.getId(),
                                              targetAngle,
                                              state.getSector());
    }

    for (auto &command : commands) {
        while (command.handle.getDevice()->isMoving()) ;
    }
}

double WingController::getAngleFromServo(handles::WingHandle &handle) const {
    auto device = handle.getDevice();
    int position = device->getPresentPosition();
    return handle.getTranslatedAngle(position == 0 ? 0 : position * 300.0 / 1023);
}

void WingController::calibrate(handles::WingHandle &handle) {
    wingStateRepository->setCurrentAngleToBaseAngle(handle.getId(), getAngleFromServo(handle));
}

void WingController::setSpeed(handles::WingHandle &handle, std::uint16_t speed, servo::Direction direction) {
    if (wingStateRepository->getState(handle.getId()).getSpeed() == speed) {
        return;
    }

    _setSpeed(handle, speed, handle.getTranslatedDirection(direction));
    wingStateRepository->updateSpeed(handle.getId(), speed);
}

void WingController::setAngle(handles::WingHandle &handle, double angle, std::uint16_t speed, bool waitForCompletion) {
    setSpeed(handle, speed);
    _setAngle(handle, handle.getTranslatedAngle(angle), waitForCompletion);
    wingStateRepository->updateAngle(handle.getId(), angle);
}

void WingController::setMode(handles::WingHandle &handle, ServoMode mode) {
    _setMode(handle, mode);
    wingStateRepository->updateMode(handle.getId(), mode);
}