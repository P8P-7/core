#include <goliath/controller/commands/shutdown_command.h>

using namespace goliath::commands;
using namespace goliath;

ShutdownCommand::ShutdownCommand(const size_t &id, std::shared_ptr<boost::asio::io_service> ioService)
        : BasicCommand(id, {}), ioService(std::move(ioService)) {
}

void ShutdownCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    ioService->stop();
    system("shutdown now > /tmp/shutdown.log&");
    BOOST_LOG_TRIVIAL(fatal) << "System is shutting down.";
}