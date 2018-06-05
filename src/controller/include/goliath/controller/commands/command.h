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

        /**
         * @return id by which the command is known in a goliath::commands::CommandMap
         */
        size_t getId() const;

        /**
         * @brief Execute a command whilst also setting its running property
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed
         */
        virtual void run(handles::HandleMap &handles, const proto::CommandMessage &message) = 0;

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
        const std::vector<size_t> &getRequiredHandles() const;

        /**
         * @brief Checks if a command can start by checking all the handles it occupies
         * @param command Command to check the occupying handles of
         * @return Whether or not all handles are free or not
         */
        virtual bool canStart(const handles::HandleMap &handles) const;

        virtual bool canRunParallel() const;

    protected:
        /**
         * @brief Represents whether a command is currently running
         */
        bool running;

        virtual void onInterrupt();

    private:
        const size_t id;

        /**
         * @brief Represents whether a command is interrupted or not
         */
        bool interrupted;

        /**
         * @brief Handle this command requires
         */
        const std::vector<size_t> requiredHandles;
    };
}
