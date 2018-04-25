#include "handle_map.h"

using namespace goliath::handles;

handle_map::handle_map(std::condition_variable &var) {
    for(int i = 0; i < MAX_HANDLES; ++i)
        map[i] = handle_mutex(&var);
}

handle_mutex *handle_map::operator[](const int index) {
    return &map[index];
}
