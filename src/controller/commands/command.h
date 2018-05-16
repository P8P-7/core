#pragma once

#include <vector>

#include <CommandMessage.pb.h>

#include "../handle_map.h"

/**
 * @file command.h
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

        /**
         * @brief Execute a command whilst also setting its running property
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed
         */
        void run(const handles::handle_map &handles, const CommandMessage &message);

        /**
         * @brief Interrupt a command by setting the interrupted property
         */
        void interrupt();
        /**
         * @brief Method to check wether a command is already interrupted
         * @return The value of interrupted
         */
        bool is_interrupted() const;

        /**
         * @brief Handle getter
         * @return The handles this specific command requires
         */
        const std::vector<size_t>& get_required_handles() const;

    protected:
        /**
         * @brief The actual execute method implemented by underlying commands
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed to implementers
         */
        virtual void execute(const handles::handle_map &handles, const CommandMessage &message) = 0;

    private:
        /**
         * @brief Represents whether a command is interrupted or not
         */
        bool interrupted = false;
        /**
         * @brief Represents whether a command is currently running
         */
        bool running = false;
        /**
         * @brief Handle this command requires
         */
        const std::vector<size_t> required_handles;
    };
}
