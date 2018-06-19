#pragma once

#include <atomic>
#include <google/protobuf/message.h>
#include <goliath/foundation/repositories/repository.h>

/**
 * @file system_status_repository.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    /**
     * @class goliath::repositories::SystemStatusRepository
     * @brief Repository containing system info
     */
    class SystemStatusRepository : public Repository {
    public:
        SystemStatusRepository();

        /**
         * @return Raspberry Pi CPU temperature
         */
        double getTempearture() const;

        void setTemperature(double temperature);

        /**
         * @return Protobuf
         */
        std::unique_ptr<google::protobuf::Message> getMessage() override;

    private:
        std::atomic<double> temperature;
    };
}

