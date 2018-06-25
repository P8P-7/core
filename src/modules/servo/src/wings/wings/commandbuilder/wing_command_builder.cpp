#include <utility>

#include <goliath/servo/wings/commandbuilder/wing_command_builder.h>
#include <goliath/servo/utilities.h>

using namespace goliath;
using namespace goliath::servo;

WingCommandBuilder::WingCommandBuilder()
        : WingCommandBuilder(nullptr, 0, 0) { }

WingCommandBuilder::WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle)
        : WingCommandBuilder(std::move(handle), 0, 0, Direction::COUNTER_CLOCKWISE) {}


WingCommandBuilder::WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle, std::uint16_t speed, double angle)
        : WingCommandBuilder(std::move(handle), speed, angle, Direction::COUNTER_CLOCKWISE) {

}

WingCommandBuilder::WingCommandBuilder(std::shared_ptr<handles::WingHandle> handle, std::uint16_t speed,
                                       double angle, Direction direction)
        : handle(std::move(handle)), angle(angle), speed(speed), direction(direction) {}

WingCommandBuilder &WingCommandBuilder::setHandle(std::shared_ptr<handles::WingHandle> handle) {
    this->handle = std::move(handle);

    return *this;
}

WingCommandBuilder &WingCommandBuilder::setAngle(double angle) {
    this->angle = angle;

    return *this;
}

WingCommandBuilder &WingCommandBuilder::setDirection(Direction direction) {
    this->direction = direction;

    return *this;
}

WingCommandBuilder &WingCommandBuilder::setShortestDirection(const WingState &state, double threshold) {
    this->direction = state.getDirectionFromWorldAngle(angle, threshold);

    return *this;
}

WingCommandBuilder &WingCommandBuilder::flipDirection() {
    this->direction = Utilities::flipDirection(this->direction);

    return *this;
}

WingCommandBuilder &WingCommandBuilder::setSpeed(std::uint16_t speed) {
    this->speed = speed;

    return *this;
}

WingCommand WingCommandBuilder::build() const {
    return {
            .handle = *handle,
            .speed = speed,
            .angle = angle,
            .direction = direction
    };
}

