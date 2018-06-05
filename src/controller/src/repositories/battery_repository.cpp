#include <goliath/controller/repositories/battery_repository.h>

#include <repositories/BatteryRepository.pb.h>

using namespace goliath;

std::unique_ptr<::google::protobuf::Message> repositories::BatteryRepository::getMessage() {
    proto::repositories::BatteryRepository repo;
    repo.set_level(getBatteryLevel());

    return std::make_unique<proto::repositories::BatteryRepository>(repo);
}

int repositories::BatteryRepository::getBatteryLevel() const {
    return std::rand() % 100 + 1;
}
