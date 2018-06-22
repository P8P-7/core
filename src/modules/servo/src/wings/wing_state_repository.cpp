#include <goliath/servo/repositories/wing_state_repository.h>
#include <boost/range/adaptor/map.hpp>

namespace protos = ::goliath::proto::repositories;
using namespace goliath::repositories;

WingStateRepository::WingStateRepository(const std::vector<size_t> &handles) {
    for (const auto &handleId : handles) {
        servo::WingState state(numberOfSectors);
        states.emplace(handleId, state);
    }
}

std::unique_ptr<::google::protobuf::Message> WingStateRepository::getMessage() {
    protos::WingStateRepository repository;
    for (const auto &wing : states) {
        auto state = repository.add_wings();
        state->set_mode(wing.second.getMode() == servo::ServoMode::JOINT
                        ? protos::WingState_Mode::WingState_Mode_JOINT
                        : protos::WingState_Mode::WingState_Mode_WHEEL);
        state->set_current_speed(wing.second.getSpeed());
        state->set_current_angle(wing.second.getWorldAngle());
    }


    return std::make_unique<protos::WingStateRepository>(repository);
}

void WingStateRepository::setCurrentAngleToBaseAngle(const size_t &handleId, const double &angle) {
    states[handleId].reset(angle);

    invalidate();
}

const goliath::servo::WingState &WingStateRepository::getState(const size_t &handleId) const {
    return states.at(handleId);
}

void WingStateRepository::updateWorldAngle(const size_t &handleId, const double &angle, const size_t &sector) {
    if (angle < 0 || angle > 300) {
        throw std::out_of_range("Angle out of range [0, 300]");
    }
    if (sector < 0 || sector >= numberOfSectors) {
        throw std::out_of_range("Sector out of range");
    }

    states[handleId].setAngle(angle);
    states[handleId].setSector(sector);

    invalidate();
}

void WingStateRepository::updateAngle(const size_t &handleId, const double &angle) {
    updateWorldAngle(handleId, angle, states[handleId].getSector());
}

void WingStateRepository::updateMode(const size_t &handleId, const goliath::servo::ServoMode &mode) {
    states[handleId].setMode(mode);

    invalidate();
}

void WingStateRepository::updateSpeed(const size_t &handleId, std::uint16_t speed) {
    states[handleId].setSpeed(speed);

    invalidate();
}

bool WingStateRepository::hasLostTracking() const {
    for (auto &kvp : states) {
        if (kvp.second.hasLostTracking()) {
            return true;
        }
    }

    return false;
}