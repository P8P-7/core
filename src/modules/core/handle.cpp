#include "handle.h"

#include <cstdlib>

using namespace goliath::handles;

Handle::Handle()
        : ownerId{0} {
}

Handle::Handle(const Handle& other)
        : locked(other.isLocked()), ownerId(other.getOwnerId()) {
}

void Handle::lock(const size_t& command_id) {
    std::lock_guard<std::mutex> lock(mutex);

    locked = true;
    ownerId = command_id;
}

void Handle::unlock() {
    std::lock_guard<std::mutex> lock(mutex);

    locked = false;
    ownerId.reset();
    var.notify_one();
}

void Handle::waitAndLock(const size_t &commandId) {
    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return !locked; });

    locked = true;
    ownerId = commandId;
}

const size_t Handle::getOwnerId() const {
    if(ownerId.is_initialized()) {
        return ownerId.get();
    }

    throw std::runtime_error("Owner is not set");
}

bool Handle::isLocked() const {
    return locked;
}
