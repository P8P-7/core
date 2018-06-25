#include <goliath/gpio/gpio_handle.h>
#include <utility>

using namespace goliath::handles;
using namespace goliath::gpio;

GPIOHandle::GPIOHandle(const size_t &id, std::shared_ptr<GPIO> device)
        : Handle(id), device(std::move(device)) {
}

std::shared_ptr<GPIO> GPIOHandle::getDevice() {
    return device;
}
