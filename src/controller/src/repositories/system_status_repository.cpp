#include <goliath/controller/repositories/system_status_repository.h>

#include <repositories/SystemStatusRepository.pb.h>

using namespace goliath::repositories;
using namespace goliath;

SystemStatusRepository::SystemStatusRepository()
        : temperature(0) { }

double SystemStatusRepository::getTempearture() const {
    return temperature;
}

void SystemStatusRepository::setTemperature(double temperature) {
    this->temperature = temperature;
}

std::unique_ptr<google::protobuf::Message> SystemStatusRepository::getMessage() {
    proto::repositories::SystemStatusRepository systemStatusRepository;

    systemStatusRepository.set_temperature(getTempearture());

    return std::make_unique<proto::repositories::SystemStatusRepository>(systemStatusRepository);
}
