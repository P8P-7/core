#pragma once

#include <boost/asio.hpp>
#include "command_map.h"
#include "handle_map.h"

/**
 * @file command_executor.h
 * @author Group 7 - Informatica
 */

using namespace goliath::handles;

namespace goliath::commands {
    /**
     * @class goliath::commands::CommandExecutor
     * @brief Takes in a command id, and executes is. Also checks handles and interrupts commands
     */
    class CommandExecutor {
    public:
        /**
         * @param commands Command Map
         * @param handles Handle Map
         */
        CommandExecutor(size_t numberOfThreads, CommandMap &commands, HandleMap &handles);

        ~CommandExecutor();

        /**
         * @brief Checks if a command is already running, if so drop it else execute it's instance by running
         * @see tryExecute. Sets status of command to running.
         * @param commandId ID of the command to run
         * @param message Arguments gathered from the protobuf input
         */
        void run(size_t commandId, const proto::CommandMessage &message);
        void run(size_t commandId);

    private:
        const size_t numberOfThreads;
        size_t numberOfActiveCommands;

        CommandMap &commands;
        HandleMap &handles;


        boost::asio::thread_pool pool;

        std::mutex mutex;

        /**
         * Running on it's own thread. Checks if the handles the command requires  are occupied. If so, call
         * interrupt() on all command's currently using the handles the new command requires. After that waits for
         * the condition variable to be triggered to then check again. If all handles are free, execute the command
         * in the current thread. Sets status to started.
         * @param commandId ID of the command to try to execute
         * @param message Arguments gathered from the protobuf input
         */
        void start(const size_t &commandId, const proto::CommandMessage &message);

        void delayedStart(const size_t &commandId, const proto::CommandMessage &message);
    };
}
