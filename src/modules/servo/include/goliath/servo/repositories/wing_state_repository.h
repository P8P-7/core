#pragma once

#include <goliath/foundation.h>
#include <repositories/WingStateRepository.pb.h>
#include "wing_state.h"
#include "../servo_mode.h"

namespace goliath::repositories {
    class WingStateRepository : public Repository {
    public:
        const size_t numberOfSectors = 4;

        explicit WingStateRepository(const std::vector<size_t> &handles);

        void updateWorldAngle(const size_t &handleId, const double &angle, const size_t &sector);
        void setCurrentAngleToBaseAngle(const size_t &handleId, const double &angle);
        void updateAngle(const size_t &handleId, const double &angle);
        void updateMode(const size_t &handleId, const servo::ServoMode &mode);
        void updateSpeed(const size_t &handleId, std::uint16_t speed);

        const servo::WingState &getState(const size_t &handleId) const;

        bool hasLostTracking() const;

        std::unique_ptr<::google::protobuf::Message> getMessage() override;
    private:
        std::map<size_t, servo::WingState> states;
    };
}

