#include <goliath/controller/repositories/system_status_repository.h>

#include <CommandMessage.pb.h>
#include <repositories/SystemStatusRepository.pb.h>

using namespace goliath::repositories;
using namespace goliath;

SystemStatusRepository::SystemStatusRepository()
        : temperature(0) { }

std::unique_ptr<google::protobuf::Message> SystemStatusRepository::getMessage() {
    proto::repositories::SystemStatusRepository systemStatusRepository;

    systemStatusRepository.set_temperature(getTemperature());

    return std::make_unique<proto::repositories::SystemStatusRepository>(systemStatusRepository);
}

size_t SystemStatusRepository::getPollingCommandId() {
    return proto::CommandMessage::kSynchronizeSystemStatusCommand;
}

double SystemStatusRepository::getTemperature() const {
    return temperature;
}

void SystemStatusRepository::setTemperature(double temperature) {
    this->temperature = temperature;

    invalidate();
}

bool SystemStatusRepository::getFanStatus() const {
    return fanEnabled;
}

void SystemStatusRepository::setFanStatus(bool enabled) {
    fanEnabled = enabled;

    invalidate();
}