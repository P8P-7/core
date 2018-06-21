#include <goliath/controller/commands/synchronize_battery_voltage_command.h>
#include <goliath/controller.h>

using namespace goliath::commands;
using namespace goliath;

SynchronizeBatteryVoltageCommand::SynchronizeBatteryVoltageCommand(const size_t &id,
                                                                   std::shared_ptr<repositories::BatteryRepository> batteryRepository)
        : BasicCommand(id,{HANDLE_LEFT_FRONT_WING_SERVO}), batteryRepository(std::move(batteryRepository)) {}

void SynchronizeBatteryVoltageCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {

    std::vector<size_t> requiredHandles = getRequiredHandles();

    int voltage = 0;
    int count = 0;

    for (auto const &servoHandle : requiredHandles) {
        voltage = getVoltage(std::static_pointer_cast<ServoHandle>(handles[servoHandle]));
        count++;
    }

    batteryRepository->setBatteryVoltage(voltage / count);
}

int SynchronizeBatteryVoltageCommand::getVoltage(const std::shared_ptr<ServoHandle> servoHandle) {
    auto servoDevice = servoHandle->getDevice();
    return servoDevice->getPresentVoltage();
}
