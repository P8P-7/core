#include "handle_map.h"

using namespace goliath::handles;

handle_map::handle_map(std::shared_ptr<std::condition_variable> var) {
    for(int i = 0; i < MAX_HANDLES; ++i) {
        map[i] = handle_mutex(var);
    }
}

std::shared_ptr<handle_mutex> handle_map::get(const size_t index) const {
    return std::shared_ptr<handle_mutex>(&map[index]);
}

std::shared_ptr<handle_mutex> handle_map::operator[](const size_t index) const {
    return get(index);
}
