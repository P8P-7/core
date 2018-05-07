#pragma once

#include <vector>
#include "../handle_map.h"

#include <Message.pb.h>

/**
 * @file command.h
 * @brief Command base class
 * @author Group 7 - Informatica
 */

 /**
  * @namespace goliath::commands
  * @brief Contains all command and command utilities
  */

namespace goliath::commands {

    /**
     * @class command
     * @brief Command base class
     */
    class command {
    public:
        explicit command(const std::vector<size_t> &required_handles);

        virtual void execute(const handles::handle_map &handles, const Message &message) = 0;

        void interrupt();
        bool is_interrupted() const;
        const std::vector<size_t>& get_required_handles() const;

    protected:
        bool interrupted = false;

        const std::vector<size_t> required_handles;
    };
}