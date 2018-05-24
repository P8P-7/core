#pragma once

#include "../handles.h"
#include "command.h"

/**
 * @file follow_line_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class FollowLineCommand : public Command {
    public:
        FollowLineCommand(const size_t &id, const boost::property_tree::ptree &config);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
        void follow_line(vision::FollowLineDetector &followLineDetector, vision::Webcam &camera);
    };
}

