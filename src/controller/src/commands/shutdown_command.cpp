#include <goliath/controller/commands/shutdown_command.h>

using namespace goliath::commands;
using namespace goliath;

ShutdownCommand::ShutdownCommand(const size_t &id, boost::asio::io_service *ioService)
        : BasicCommand(id, {}), ioService(std::move(ioService)) {
}

void ShutdownCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    ioService->stop();
    system("sleep 5; shutdown -P -t now > /tmp/shutdown.log&");
    BOOST_LOG_TRIVIAL(fatal) << "Machine is powering down in 5 seconds.";
}