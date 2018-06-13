#pragma once

#include <goliath/vision.h>

#include "../handles.h"
#include "basic_command.h"

/**
 * @file follow_line_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    /**
     * @class goliath::commands::WunderhornCommand
     * @brief Command that follows a line and pauses on a coled area in the center
     */
    class WunderhornCommand : public BasicCommand {
    public:
        /**
         * @param id Command ID
         * @param config Optional configuration
         */
        explicit WunderhornCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
        /**
         * @brief Follows a straight line until it stops
         * @param followLineDetector Detector
         * @param camera Webcam
         */
        void follow_line(vision::FollowLineDetector &followLineDetector, vision::Webcam &camera);
    };
}

