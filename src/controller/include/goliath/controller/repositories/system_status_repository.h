#pragma once

#include <atomic>

#include <google/protobuf/message.h>
#include <goliath/foundation/repositories/polling_repository.h>

/**
 * @file system_status_repository.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    /**
     * @class goliath::repositories::SystemStatusRepository
     * @brief Repository containing system info
     */
    class SystemStatusRepository : public PollingRepository {
    public:
        SystemStatusRepository();

        /**
         * @return Raspberry Pi CPU temperature
         */
        double getTemperature() const;

        void setTemperature(double temperature);

        bool getFanStatus() const;

        void setFanStatus(bool enabled);

        size_t getPollingCommandId() override;

        /**
         * @return Protobuf
         */
        std::unique_ptr<google::protobuf::Message> getMessage() override;

    private:
        std::atomic<double> temperature;
        std::atomic<bool> fanEnabled;
    };
}

