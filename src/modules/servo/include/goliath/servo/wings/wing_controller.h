#pragma once

#include <goliath/servo/repositories/wing_state_repository.h>
#include <goliath/servo/wings/wing_command.h>
#include <goliath/servo/wing_handle.h>
#include <goliath/servo/servo_mode.h>

namespace goliath::servo {
    class WingController {
    public:
        explicit WingController(std::shared_ptr<repositories::WingStateRepository> wingStateRepository);

        void setWorldAngle(handles::WingHandle &handle, double angle, std::uint16_t speed,
                           Direction direction = Direction::CLOCKWISE);

        void setAngle(handles::WingHandle &handle, double angle, std::uint16_t speed, bool waitForCompletion = true);

        void setMode(handles::WingHandle &handle, ServoMode mode);

        void setSpeed(handles::WingHandle &handle, std::uint16_t speed,
                      servo::Direction direction = servo::Direction::COUNTER_CLOCKWISE);

        double getAngleFromServo(handles::WingHandle &handle) const;
        void calibrate(handles::WingHandle &handle);

        void execute(std::vector<servo::WingCommand> commands);

    private:
        friend class DeadzoneTransitionController;

        std::shared_ptr<repositories::WingStateRepository> wingStateRepository;

        void _setAngle(handles::WingHandle &handle, double angle, bool waitForCompletion = true);

        void _setMode(handles::WingHandle &handle, ServoMode mode);

        void _setSpeed(handles::WingHandle &handle,
                       std::uint16_t speed,
                       servo::Direction direction = servo::Direction::COUNTER_CLOCKWISE);
    };
}