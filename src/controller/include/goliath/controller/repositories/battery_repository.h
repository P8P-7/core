#pragma once

#include <google/protobuf/message.h>
#include <goliath/foundation/repositories/repository.h>

/**
 * @file battery_repository.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    /**
     * @class goliath::repositories::BatteryRepository
     * @brief Repository containing battery level
     */
    class BatteryRepository : public Repository {
    public:
        /**
         * @return Battery level
         */
        int getBatteryLevel() const;

        /**
        * @return Battery voltage
        */
        int getBatteryVoltage() const;

        void setBatteryVoltage(int voltage);

        /**
         * @return Protobuf
         */
        std::unique_ptr<::google::protobuf::Message> getMessage() override;

    private:
        int voltage;
        const int minVoltage = 105;
        const int maxVoltage = 125;
    };
}

