#include <repositories/BatteryRepository.pb.h>
#include "battery_repository.h"

using namespace goliath::repositories;

std::unique_ptr<::google::protobuf::Message> battery_repository::get_message() {
    BatteryRepository repo;
    repo.set_level(get_battery_level());

    return std::make_unique<BatteryRepository>(repo);
}

int battery_repository::get_battery_level() const {
    return std::rand() % 100 + 1;
}
