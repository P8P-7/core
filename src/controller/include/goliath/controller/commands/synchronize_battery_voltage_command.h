#pragma once

#include <goliath/servo.h>
#include "basic_command.h"
#include "../handles.h"
#include "../repositories/battery_repository.h"

/**
 * @file synchronize_battery_voltage_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class SynchronizeBatteryVoltageCommand : public BasicCommand {
    public:
        SynchronizeBatteryVoltageCommand(const size_t &id,
                                         std::shared_ptr<repositories::BatteryRepository> batteryRepository);

    private:
        std::shared_ptr<repositories::BatteryRepository> batteryRepository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        int GetVoltage(std::shared_ptr<handles::ServoHandle> servoHandle);
    };
}
