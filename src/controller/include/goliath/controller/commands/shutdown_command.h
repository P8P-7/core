#pragma once

#include "../watcher.h"

#include <goliath/zmq-messaging.h>
#include <boost/asio/io_service.hpp>

#include "basic_command.h"

/**
 * @file shutdown_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class ShutdownCommand : public BasicCommand {
    public:
        ShutdownCommand(const size_t &id, std::shared_ptr<boost::asio::io_service> ioService);

    private:
        std::shared_ptr<boost::asio::io_service> ioService;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
