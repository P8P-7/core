#pragma once

#include <goliath/gpio.h>
#include <goliath/servo/wings/wing_command.h>
#include <goliath/servo/wings/wing_controller.h>
#include <goliath/servo/repositories/wing_state_repository.h>

#include "basic_command.h"
#include "../handles.h"

/**
 * @file line_dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class LineDanceCommand : public BasicCommand {
    public:
        explicit LineDanceCommand(const size_t &id,
                                  const std::shared_ptr<repositories::WingStateRepository> &repository);

        void processPulse();

        void startBeat();

        void waitForBeat();

        void listenGpio(const std::shared_ptr<gpio::GPIO> &gpioDevice);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        std::chrono::high_resolution_clock::time_point t0;
        std::chrono::high_resolution_clock::time_point t1;

        /**
         * @brief Represents whether a beat has been started or not
         */
        std::atomic<bool> beatStarted;

        std::mutex mutex;

        /**
         * @brief Condition variable that can be waited on
         */
        std::condition_variable beatStart;

        /**
         * @brief Used to calculate the running BPM
         */
        std::deque<double> history;

        std::atomic<bool> hasBeat;

        /**
         * @brief Running BPM
         */
        std::atomic<double> runningBpm{0.0};

        /**
         * @brief Smallest possible BPM allowed
         */
        double minimumAllowedBpm = 40.0;

        /**
         * @brief Largest possible BPM allowed
         */
        double maximumAllowedBpm = 120.0;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
