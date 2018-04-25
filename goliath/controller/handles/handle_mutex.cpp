#include "handle_mutex.h"

using namespace goliath::handles;

void handle_mutex::lock() {
    locked = true;
}

void handle_mutex::unlock() {
    locked = false;
    var->notify_one();
}

bool handle_mutex::get() {
    return locked;
}