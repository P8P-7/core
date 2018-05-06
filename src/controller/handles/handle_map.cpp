#include "handle_map.h"

using namespace goliath::handles;

handle_map::handle_map() {
    for(int i = 0; i < MAX_HANDLES; ++i) {
        handle_mutex h_mutex;
        map[i] = std::make_shared<handle_mutex>(h_mutex);
    }
}

std::shared_ptr<handle_mutex> handle_map::get(const size_t index) const {
    return map[index];
}

std::shared_ptr<handle_mutex> handle_map::operator[](const size_t index) const {
    return get(index);
}
