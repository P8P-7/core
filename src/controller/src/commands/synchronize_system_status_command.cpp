#include <goliath/controller/commands/synchronize_system_status_command.h>

#include <fstream>

using namespace goliath::commands;
using namespace goliath;

const std::string SynchronizeSystemStatusCommand::TEMPERATURE_FILE = "/sys/devices/virtual/thermal/thermal_zone0/temp";

SynchronizeSystemStatusCommand::SynchronizeSystemStatusCommand(const size_t &id,
                                                               std::shared_ptr<repositories::SystemStatusRepository> systemStatusRepository)
    : BasicCommand(id, {}), systemStatusRepository(std::move(systemStatusRepository)) { }

void SynchronizeSystemStatusCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    std::ifstream tempFile(TEMPERATURE_FILE);

    std::string line;
    std::getline(tempFile, line);

    if (isInterrupted()) {
        return;
    }

    double temperature = std::stod(line) / 1000.0;

    systemStatusRepository->setTemperature(temperature);
    systemStatusRepository->invalidate();
}
