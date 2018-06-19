#include <goliath/controller/commands/line_dance_command.h>
#include <goliath/gpio.h>

using namespace goliath;
using namespace goliath::handles;

// In milliseconds
const int pollingRate = 5;

commands::LineDanceCommand::LineDanceCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_GPIO_PIN_5}) {
}

void commands::LineDanceCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has started";
    std::shared_ptr<gpio::GPIO> gpioDevice = std::static_pointer_cast<GPIOHandle>(handles[HANDLE_GPIO_PIN_5])->getDevice();

    while (!isInterrupted()) {
        bool pulse = gpioDevice->get() != 0;

        // TODO: Do something with the pulse that we receive.
        BOOST_LOG_TRIVIAL(debug) << "Pulse" << pulse;

        std::this_thread::sleep_for(std::chrono::milliseconds(pollingRate));
    }

    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has finished";
}
