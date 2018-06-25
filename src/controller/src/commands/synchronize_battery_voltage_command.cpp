#include <goliath/controller/commands/synchronize_battery_voltage_command.h>
#include <goliath/controller.h>

using namespace goliath::commands;
using namespace goliath;

SynchronizeBatteryVoltageCommand::SynchronizeBatteryVoltageCommand(const size_t &id,
                                                                   std::shared_ptr<repositories::BatteryRepository> batteryRepository)
        : BasicCommand(id,{HANDLE_LEFT_FRONT_WING_SERVO}), batteryRepository(std::move(batteryRepository)) {}

void SynchronizeBatteryVoltageCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    return;
    
    int voltage = getVoltage(handles.get<ServoHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    BOOST_LOG_TRIVIAL(trace) << "Received voltage " << std::to_string(voltage);
    if(voltage >= batteryRepository->getBatteryMinVoltage() - 1 && voltage <= batteryRepository->getBatteryMaxVoltage()) {
        BOOST_LOG_TRIVIAL(trace) << "Valid voltage";
        batteryRepository->setBatteryVoltage(voltage);
    } else {
        BOOST_LOG_TRIVIAL(trace) << "Invalid voltage";
    }
}

int SynchronizeBatteryVoltageCommand::getVoltage(const std::shared_ptr<ServoHandle> servoHandle) {
    auto servoDevice = servoHandle->getDevice();
    return servoDevice->getPresentVoltage();
}
