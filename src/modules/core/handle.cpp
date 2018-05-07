#include <goliath/handle.h>

using namespace goliath::handles;

handle::handle()
        : owner_id{0} {
}

handle::handle(const handle& other)
        : locked(other.is_locked()), owner_id(other.get_owner_id()) {
}

void handle::lock(const size_t& command_id) {
    locked = true;
    owner_id = command_id;
}

void handle::unlock() {
    locked = false;
    owner_id = 0;
    var.notify_one();
}

void handle::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return !locked; });
}

const size_t handle::get_owner_id() const {
    return owner_id;
}

bool handle::is_locked() const {
    return locked;
}