#include <goliath/core.h>
#include "handle.h"

#include <cstdlib>

using namespace goliath::handles;

Handle::Handle(const size_t &handle_id)
        : handleId(handle_id), ownerId(0) {
}

Handle::Handle(const Handle &other)
        : handleId(other.getHandleId()), ownerId(other.getOwnerId()) {
}

void Handle::lock(const size_t& commandId) {
    std::lock_guard<std::mutex> lock(mutex);
    if (isLocked()) {
        throw exceptions::HandleError(getHandleId(), "Could not be locked because it was already in use.");
    }

    ownerId = commandId;
}

void Handle::unlock() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!isLocked()) {
        throw exceptions::HandleError(getHandleId(), "Could not be unlocked because it wasn't locked.");
    }

    ownerId.reset();

    var.notify_one();
}

void Handle::waitAndLock(const size_t &commandId) {
    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return !isLocked(); });

    ownerId = commandId;
}

const size_t Handle::getOwnerId() const {
    if(ownerId.is_initialized()) {
        return ownerId.get();
    }

    throw std::runtime_error("Owner is not set");
}

const size_t Handle::getHandleId() const {
    return handleId;
}

bool Handle::isLocked() const {
    return ownerId.is_initialized();
}