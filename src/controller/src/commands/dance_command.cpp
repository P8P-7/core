#include <goliath/controller/commands/dance_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>

#include <goliath/emotions.h>

using namespace goliath::handles;
using namespace goliath;

commands::DanceCommand::DanceCommand(const size_t& id)
    : BasicCommand(id, {HANDLE_EMOTIONS}) {
}

void commands::DanceCommand::execute(HandleMap& handles, const proto::CommandMessage& message) {
    std::shared_ptr<emotions::EmotionPublisher> publisher = std::static_pointer_cast<EmotionHandle>(
        handles[HANDLE_CAM])->getPublisher();

    // TODO: Implement dance
}
