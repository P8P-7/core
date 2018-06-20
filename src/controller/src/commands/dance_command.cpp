#include <goliath/controller/commands/dance_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>

#include <goliath/emotions.h>
#include <goliath/led-strip-controller.h>

using namespace goliath::handles;
using namespace goliath;

commands::DanceCommand::DanceCommand(const size_t &id)
    : BasicCommand(id, {HANDLE_EMOTIONS, HANDLE_LED_CONTROLLER, HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                        HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR, HANDLE_RIGHT_FRONT_MOTOR,
                        HANDLE_RIGHT_BACK_MOTOR, HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                        HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}) {}

void commands::DanceCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    using Emotion = proto::repositories::EmotionRepository;

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

    std::shared_ptr<repositories::EmotionRepository> emotionRepository = handles.get<handles::EmotionHandle>(
        HANDLE_EMOTIONS)->getEmotionRepository();

    // Phase 1: Happy
    BOOST_LOG_TRIVIAL(info) << "Starting phase 1 of dance: Happy";
    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
    ledController.sendCommand(allGreen);
    // TODO: Spin for 30 sec + improvise
    // TODO: Move arm up and down

    // Phase 2: Angry
    BOOST_LOG_TRIVIAL(info) << "Starting phase 2 of dance: Angry";
    // TODO: Move wings up
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
    // TODO: Drive forwards and backwards
    // TODO: Turn towards jury (or any)
    // TODO: Wave
    emotionRepository->setCurrentEmotion(Emotion::WINK_RIGHT);

    // Phase 4: Neutraal
    BOOST_LOG_TRIVIAL(info) << "Starting phase 2 of dance: Neutral";
    emotionRepository->setCurrentEmotion(Emotion::NEUTRAL);
    ledController.sendCommand(allYellow);
    // TODO: Turn 360 degrees
    // TODO: Improvise
    // TODO: Turn towards jury
    emotionRepository->setCurrentEmotion(Emotion::WINK_LEFT);
}
