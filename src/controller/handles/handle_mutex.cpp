#include "handle_mutex.h"

using namespace goliath::handles;

void handle_mutex::lock() {
    locked = true;
}

void handle_mutex::unlock() {
    locked = false;
    var.lock()->notify_one();
}

void handle_mutex::set_locker(const unsigned& command_id) {
    locker_id = command_id;
}

const unsigned handle_mutex::get_locker() const {
    return locker_id;
}

bool handle_mutex::get_status() const {
    return locked;
}