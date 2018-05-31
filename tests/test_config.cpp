#define BOOST_TEST_MODULE test_config

#include <vector>
#include <fstream>
#include <iostream>
#include <boost/test/included/unit_test.hpp>

#include <repositories/ConfigRepository.pb.h>
#include <google/protobuf/util/json_util.h>

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_export_config) {
        std::string jsonString;

        ConfigRepository configRepository;

        google::protobuf::util::JsonPrintOptions options;
        options.add_whitespace = true;
        options.always_print_primitive_fields = true;
        options.preserve_proto_field_names = true;

        auto *zmqConfig = new ZmqConfig;
        zmqConfig->set_publisher_port(5556);
        zmqConfig->set_subscriber_port(5555);

        auto *serialConfig = new SerialConfig;
        serialConfig->set_port("/dev/serial0");
        serialConfig->set_baudrate(1000000);

        auto *gpioConfig = new GpioConfig;
        gpioConfig->set_pin(18);

        auto *visionConfig = new VisionConfig;
        visionConfig->set_webcam(0);

        auto *servoConfig = new ServoConfig;
        Wing* leftFrontWing = servoConfig->add_wings();
        leftFrontWing->set_position(Position::LEFT_FRONT);
        leftFrontWing->set_id(1);

        Wing* leftBackWing = servoConfig->add_wings();
        leftBackWing->set_position(Position::LEFT_BACK);
        leftBackWing->set_id(2);

        Wing* rightFrontWing = servoConfig->add_wings();
        rightFrontWing->set_position(Position::RIGHT_FRONT);
        rightFrontWing->set_id(3);

        Wing* rightBackWing = servoConfig->add_wings();
        rightBackWing->set_position(Position::RIGHT_BACK);
        rightBackWing->set_id(4);

        auto *i2cConfig = new I2cConfig;
        i2cConfig->set_device("/dev/i2c-1");

        auto *motorControllerConfig = new MotorControllerConfig;
        motorControllerConfig->set_address("0x30");

        Motor* leftFrontMotor = motorControllerConfig->add_motors();
        leftFrontMotor->set_position(Position::LEFT_FRONT);
        leftFrontMotor->set_id(0);

        Motor* leftBackMotor = motorControllerConfig->add_motors();
        leftBackMotor->set_position(Position::LEFT_BACK);
        leftBackMotor->set_id(1);

        Motor* rightFrontMotor = motorControllerConfig->add_motors();
        rightFrontMotor->set_position(Position::RIGHT_FRONT);
        rightFrontMotor->set_id(2);

        Motor* rightBackMotor = motorControllerConfig->add_motors();
        rightBackMotor->set_position(Position::RIGHT_BACK);
        rightBackMotor->set_id(3);

        auto *emotionsConfig = new EmotionConfig;
        emotionsConfig->set_host("localhost");
        emotionsConfig->set_port(5558);

        auto *commandExecutorConfig = new CommandExecutorConfig;
        commandExecutorConfig->set_number_of_executors(4);

        auto *watcherConfig = new WatcherConfig;
        watcherConfig->set_polling_rate(250);

        auto *loggingConfig = new LoggingConfig;
        loggingConfig->set_severity_level(LoggingConfig_Severity_TRACE);

        configRepository.set_allocated_zmq(zmqConfig);
        configRepository.set_allocated_serial(serialConfig);
        configRepository.set_allocated_gpio(gpioConfig);
        configRepository.set_allocated_vision(visionConfig);
        configRepository.set_allocated_servos(servoConfig);
        configRepository.set_allocated_i2c(i2cConfig);
        configRepository.set_allocated_motor_controller(motorControllerConfig);
        configRepository.set_allocated_emotions(emotionsConfig);
        configRepository.set_allocated_command_executor(commandExecutorConfig);
        configRepository.set_allocated_watcher(watcherConfig);
        configRepository.set_allocated_logging(loggingConfig);

        google::protobuf::util::MessageToJsonString(configRepository, &jsonString, options);

        std::ofstream jsonOut(GOLIATH_TEST_CONFIG_OUTPUT);
        jsonOut << jsonString;
        jsonOut.close();
    }

BOOST_AUTO_TEST_SUITE_END()
