#include <goliath/controller/commands/dance_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <commands/MoveCommand.pb.h>

#include <goliath/controller/commands/move_command.h>
#include <goliath/motor-controller.h>
#include <goliath/emotions.h>
#include <goliath/led-strip-controller.h>
#include <goliath/transitions.h>

using namespace goliath::handles;
using namespace goliath;

commands::DanceCommand::DanceCommand(const size_t &id)
    : BasicCommand(id, {HANDLE_EMOTIONS, HANDLE_LED_CONTROLLER, HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                        HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR, HANDLE_RIGHT_FRONT_MOTOR,
                        HANDLE_RIGHT_BACK_MOTOR, HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                        HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}) {}

void commands::DanceCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    using namespace std::chrono_literals;
    using Emotion = proto::repositories::EmotionRepository;
    using Motor = proto::commands::MotorCommand;

    controller::LedStatus allColor{
        .lightingType = controller::LightingType::ALL,
        .colorType = controller::ColorType::HSV
    };

    controller::AllLedsMessage allGreen{
        allColor,
        .allLeds = {
            .hue = 125,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allRed{
        allColor,
        .allLeds = {
            .hue = 0,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allBlue{
        allColor,
        .allLeds = {
            .hue = 236,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allYellow{
        allColor,
        .allLeds = {
            .hue = 59,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    controller::LedStripController ledController(ledControllerSlave);

    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    std::shared_ptr<repositories::EmotionRepository> emotionRepository = handles.get<handles::EmotionHandle>(
        HANDLE_EMOTIONS)->getEmotionRepository();

    // Phase 1: Happy
    BOOST_LOG_TRIVIAL(info) << "Starting phase 1 of dance: Happy";
    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
    ledController.sendCommand(allGreen);
    // TODO: Move wings up
    auto spinUpPhase = std::make_shared<transitions::Phase>(15000ms, 20, 0, 127, &transitions::methods::easeInOutCubic);
    auto spinDownPhase = std::make_shared<transitions::Phase>(15000ms, 20, 127, 0, &transitions::methods::easeInOutCubic);

    transitions::Reel spinUpReel(20, [&motorController](double value){
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = Motor::RIGHT_FRONT,
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = static_cast<motor_controller::MotorSpeed>(value)
        });
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = Motor::RIGHT_BACK,
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = static_cast<motor_controller::MotorSpeed>(value)
        });
    });

    transitions::Reel spinDownReel = spinUpReel;
    // TODO: Move arm up and down
    transitions::TransitionExecutor::execute(std::make_shared<transitions::Reel>(spinUpReel));
    transitions::TransitionExecutor::execute(std::make_shared<transitions::Reel>(spinDownReel));

    // Phase 2: Angry
    BOOST_LOG_TRIVIAL(info) << "Starting phase 2 of dance: Angry";
    // TODO: Move wings diagonal
    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    ledController.sendCommand(allRed);
    // TODO: Move arm up
    // TODO: Drive to edge
    // TODO: Turn 90 degrees
    emotionRepository->setCurrentEmotion(Emotion::SUPRISED);
    // TODO: Move arm down
    // TODO: Move

    // Phase 3: Scared
    BOOST_LOG_TRIVIAL(info) << "Starting phase 2 of dance: Sad";
    // TODO: Turn 90 degrees
    emotionRepository->setCurrentEmotion(Emotion::SAD);
    ledController.sendCommand(allBlue);

    for (int i = 0; i < 4; ++i) {
        for (auto motor : MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
            motorController.sendCommand(motor_controller::MotorStatus{
                .id = static_cast<motor_controller::MotorId>(motor.first),
                .direction = motor_controller::MotorDirection::FORWARDS,
                .speed = 127
            });

            std::this_thread::sleep_for(1s);

            motorController.sendCommand(motor_controller::MotorStatus{
                .id = static_cast<motor_controller::MotorId>(motor.first),
                .direction = motor_controller::MotorDirection::BACKWARDS,
                .speed = 127
            });
        }
    }

    // TODO: Turn towards jury (or any)
    // TODO: Wave
    emotionRepository->setCurrentEmotion(Emotion::WINK_RIGHT);

    // Phase 4: Neutral
    BOOST_LOG_TRIVIAL(info) << "Starting phase 2 of dance: Neutral";
    emotionRepository->setCurrentEmotion(Emotion::NEUTRAL);
    ledController.sendCommand(allYellow);
    // TODO: Turn 360 degrees
    // TODO: Improvise
    // TODO: Turn towards jury
    emotionRepository->setCurrentEmotion(Emotion::WINK_LEFT);
}
