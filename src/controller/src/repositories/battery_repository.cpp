#include <goliath/controller/repositories/battery_repository.h>

#include <repositories/BatteryRepository.pb.h>

using namespace goliath;

std::unique_ptr<::google::protobuf::Message> repositories::BatteryRepository::getMessage() {
    proto::repositories::BatteryRepository batteryRepository;

    batteryRepository.set_level(getBatteryLevel());

    return std::make_unique<proto::repositories::BatteryRepository>(batteryRepository);
}

int repositories::BatteryRepository::getBatteryLevel() const {
    int level = (voltage - minVoltage) * 100 / (maxVoltage - minVoltage);
    return std::min(100, std::max(0, level));
}

int repositories::BatteryRepository::getBatteryVoltage() const {
    return voltage;
}

void repositories::BatteryRepository::setBatteryVoltage(int voltage) {
    this->voltage = voltage;
    this->invalidate();
}

int repositories::BatteryRepository::getBatteryMinVoltage() const {
    return minVoltage;
}

int repositories::BatteryRepository::getBatteryMaxVoltage() const {
    return maxVoltage;
}

size_t repositories::BatteryRepository::getPollingCommandId() {
    return proto::CommandMessage::kSynchronizeBatteryVoltageCommand;
}
