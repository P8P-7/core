#include "handle_mutex.h"

using namespace goliath::handles;

void handle_mutex::lock() {
    locked = true;
}

void handle_mutex::unlock() {
    locked = false;
    var.lock()->notify_one();
}

const unsigned handle_mutex::get_caller() const {
    return caller;
}

void handle_mutex::set_caller(const unsigned& command_id) {
    caller = command_id;
}

bool handle_mutex::operator()() const {
    return locked;
}
