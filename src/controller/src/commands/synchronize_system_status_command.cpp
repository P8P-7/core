#include <goliath/controller/commands/synchronize_system_status_command.h>

#include <fstream>

using namespace goliath::commands;
using namespace goliath;

const std::string SynchronizeSystemStatusCommand::TEMPERATURE_FILE = "/sys/devices/virtual/thermal/thermal_zone0/temp";

SynchronizeSystemStatusCommand::SynchronizeSystemStatusCommand(const size_t &id,
                                                               std::shared_ptr<repositories::SystemStatusRepository> systemStatusRepository,
                                                               const std::chrono::milliseconds &interval)
    : BasicCommand(id, {}), systemStatusRepository(std::move(systemStatusRepository)), interval(interval) {}

void SynchronizeSystemStatusCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    while (true) {
        if (isInterrupted()) {
            break;
        }

        std::ifstream tempFile(TEMPERATURE_FILE);

        std::string line;
        std::getline(tempFile, line);

        double temperature = std::stod(line) / 1000.0;

        systemStatusRepository->setTemperature(temperature);
        systemStatusRepository->invalidate();

        std::this_thread::sleep_for(interval);
    }
}
