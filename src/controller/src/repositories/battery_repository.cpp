#include <goliath/controller/repositories/battery_repository.h>

#include <repositories/BatteryRepository.pb.h>

using namespace goliath;

std::unique_ptr<::google::protobuf::Message> repositories::BatteryRepository::getMessage() {
    proto::repositories::BatteryRepository repo;

    return std::make_unique<proto::repositories::BatteryRepository>(repo);
}

int repositories::BatteryRepository::getBatteryLevel() const {
    return (voltage - minVoltage) * 100 / (maxVoltage - minVoltage);
}

int repositories::BatteryRepository::getBatteryVoltage() const {
    return voltage;
}

void repositories::BatteryRepository::setBatteryVoltage(int voltage) {
    this->voltage = voltage;
    this->invalidate();
}