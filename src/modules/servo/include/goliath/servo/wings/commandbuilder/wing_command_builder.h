#pragma once

#include <goliath/servo/repositories/wing_state_repository.h>
#include <goliath/servo/wings/wing_command.h>
#include <goliath/servo/wing_handle.h>

namespace goliath::servo {
    class WingCommandBuilder {
    public:
        WingCommandBuilder();
        explicit WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle);
        WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle, std::uint16_t speed, double angle);
        WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle, std::uint16_t speed, double angle, Direction direction);

        WingCommandBuilder &setHandle(std::shared_ptr<handles::WingHandle> handle);
        WingCommandBuilder &setAngle(double angle);
        WingCommandBuilder &setDirection(Direction direction);

        WingCommandBuilder &setShortestDirection(const WingState &state, double threshold = 180);

        WingCommandBuilder &flipDirection();
        WingCommandBuilder &setSpeed(std::uint16_t speed);

        std::shared_ptr<handles::WingHandle> getHandle() const;

        WingCommand build() const;

    private:
        std::shared_ptr<handles::WingHandle> handle;
        double angle;
        std::uint16_t speed;
        Direction direction;
    };
}
