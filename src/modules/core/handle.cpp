#include <goliath/core.h>
#include "handle.h"

using namespace goliath::handles;

handle::handle(const size_t &handle_id)
        : handle_id(handle_id), owner_id(0) {
}

handle::handle(const handle &other)
        : handle_id(other.get_handle_id()), owner_id(other.get_owner_id()) {
}

void handle::lock(const size_t &command_id) {
    std::lock_guard<std::mutex> lock(mutex);
    if (is_locked()) {
        throw exceptions::HandleError(get_handle_id(), "Could not be locked because it was already in use.");
    }

    owner_id = command_id;
}

void handle::unlock() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!is_locked()) {
        throw exceptions::HandleError(get_handle_id(), "Could not be unlocked because it wasn't locked.");
    }

    owner_id.reset();
    var.notify_one();
}

void handle::wait_and_lock(const size_t &command_id) {
    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return !is_locked(); });

    owner_id = command_id;
}

const size_t handle::get_owner_id() const {
    if (owner_id.is_initialized()) {
        return owner_id.get();
    }

    throw std::runtime_error("Owner is not set");
}

const size_t handle::get_handle_id() const {
    return handle_id;
}

bool handle::is_locked() const {
    return owner_id.is_initialized();
}