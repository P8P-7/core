#include <goliath/controller/commands/synchronize_battery_voltage_command.h>
#include <goliath/controller.h>

using namespace goliath::commands;
using namespace goliath;

SynchronizeBatteryVoltageCommand::SynchronizeBatteryVoltageCommand(const size_t &id,
                                                                   std::shared_ptr<repositories::BatteryRepository> batteryRepository)
        : BasicCommand(id,{HANDLE_LEFT_FRONT_WING_SERVO}), batteryRepository(std::move(batteryRepository)) {}

void SynchronizeBatteryVoltageCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {

    std::vector<size_t> requiredHandles = getRequiredHandles();

    double voltageTotal = 0;
    int count = 0;

    for (auto const &servoHandle : requiredHandles) {
        int voltage = getVoltage(handles.get<ServoHandle>(servoHandle));
        if(voltage >= batteryRepository->getBatteryMinVoltage() - 1 && voltage <= batteryRepository->getBatteryMaxVoltage()){
            voltageTotal += voltage;
            count++;
        }
    }

    if(count > 0){
        batteryRepository->setBatteryVoltage(static_cast<int>(voltageTotal / count));
    }
}

int SynchronizeBatteryVoltageCommand::getVoltage(const std::shared_ptr<ServoHandle> servoHandle) {
    auto servoDevice = servoHandle->getDevice();
    return servoDevice->getPresentVoltage();
}
