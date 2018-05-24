#pragma once

#include <boost/property_tree/ptree.hpp>
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
     * @class goliath::commands::Command
     * @brief Command base class
     */
    class Command {
    public:
        Command(const size_t &id, const std::vector<size_t> &requiredHandles);
        Command(const size_t &id, const std::vector<size_t> &requiredHandles, const boost::property_tree::ptree &config);

        /**
         * @return id by which the command is known in a goliath::commands::CommandMap
         */
        size_t getId() const;

        /**
         * @brief Execute a command whilst also setting its running property
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed
         */
        void run(const handles::HandleMap &handles, const CommandMessage &message);

        /**
         * @brief Interrupt a command by setting the interrupted property
         */
        void interrupt();
        /**
         * @brief Method to check wether a command is already interrupted
         * @return The value of interrupted
         */
        bool isInterrupted() const;

        /**
         * @brief Handle getter
         * @return The handles this specific command requires
         */
        const std::vector<size_t>& getRequiredHandles() const;

    protected:
        /**
         * @brief The actual execute method implemented by underlying commands
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed to implementers
         */
        virtual void execute(const handles::HandleMap &handles, const CommandMessage &message) = 0;
        boost::property_tree::ptree config;

    private:
        const size_t id;

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
        const std::vector<size_t> requiredHandles;
    };
}
