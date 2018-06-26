#include <goliath/controller/commands/synchronize_system_status_command.h>

#include <fstream>
#include <goliath/gpio.h>

using namespace goliath::commands;
using namespace goliath;

const std::string SynchronizeSystemStatusCommand::TEMPERATURE_FILE = "/sys/devices/virtual/thermal/thermal_zone0/temp";

SynchronizeSystemStatusCommand::SynchronizeSystemStatusCommand(const size_t &id,
                                                               std::shared_ptr<repositories::SystemStatusRepository> systemStatusRepository,
                                                               double enableFanThreshold,
                                                               double disableFanThreshold)
        : BasicCommand(id, {HANDLE_GPIO_PIN_FAN}),
          systemStatusRepository(std::move(systemStatusRepository)),
          enableFanThreshold(enableFanThreshold),
          disableFanThreshold(disableFanThreshold) {}

void SynchronizeSystemStatusCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    std::ifstream tempFile(TEMPERATURE_FILE);

    std::string line;
    std::getline(tempFile, line);

    double temperature = std::stod(line) / 1000.0;
    systemStatusRepository->setTemperature(temperature);

    bool enableFan = false;
    if (temperature > enableFanThreshold) {
        enableFan = true;
    } else if (systemStatusRepository->getFanStatus() && temperature < disableFanThreshold) {
        enableFan = false;
    } else if (!systemStatusRepository->getFanStatus() && temperature < enableFanThreshold) {
        enableFan = false;
    }

    if (enableFan != systemStatusRepository->getFanStatus()) {
        systemStatusRepository->setFanStatus(enableFan);
    }
    auto device = handles.get<handles::GPIOHandle>(HANDLE_GPIO_PIN_FAN)->getDevice();
    device->set(enableFan ? gpio::GPIO::State::Low : gpio::GPIO::State::High);
}
