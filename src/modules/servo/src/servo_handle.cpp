#include <goliath/servo/servo_handle.h>

using namespace goliath::handles;
using namespace goliath::dynamixel;

ServoHandle::ServoHandle(const size_t &id, std::shared_ptr<Dynamixel> device)
        : Handle(id), device(device) { }

std::shared_ptr<Dynamixel> ServoHandle::getDevice() {
    return device;
}
