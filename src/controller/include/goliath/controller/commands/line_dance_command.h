#pragma once

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
        explicit LineDanceCommand(const size_t &id, const std::shared_ptr<repositories::WingStateRepository> &repository);

        void processPulse(servo::WingController &wingController, std::vector<servo::WingCommand> commands);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        std::chrono::high_resolution_clock::time_point t0;
        std::chrono::high_resolution_clock::time_point t1;

        // Used to calculate the running BPM
        std::vector<double> history;

        // Running BPM
        double runningBpm = 0.0;

        // Smallest possible BPM allowed,
        double minimumAllowedBpm = 60.0;

        // Largest possible BPM allowed
        double maximumAllowedBpm = 180.0;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
