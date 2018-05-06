#include "handle_mutex.h"

using namespace goliath::handles;

handle_mutex::handle_mutex()
        : locker_id{0} {
}

handle_mutex::handle_mutex(const handle_mutex& other)
        : locked(other.is_locked()), locker_id(other.get_locker()) {
}

void handle_mutex::lock() {
    locked = true;
}

void handle_mutex::unlock() {
    locked = false;
    var.notify_one();
}

void handle_mutex::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    var.wait(lock, [&]() { return !locked; });
}

void handle_mutex::set_locker(const unsigned& command_id) {
    locker_id = command_id;
}

const unsigned handle_mutex::get_locker() const {
    return locker_id;
}

bool handle_mutex::is_locked() const {
    return locked;
}