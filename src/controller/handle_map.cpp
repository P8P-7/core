#include <vector>
#include <algorithm>
#include "handle_map.h"

using namespace goliath::handles;

handle_map::handle_map() {}
handle_map::handle_map(const std::map<size_t, std::shared_ptr<handle>> &map) : map(map) {}

handle_map::~handle_map() {
    for (auto kvp : map) {
        kvp.second->unlock();
    }
}

handle_map handle_map::get_handles(const std::vector<size_t> &handles) const {
    handle_map selection{};

    for (auto kvp : map) {
        auto result = std::find(handles.begin(), handles.end(), kvp.first);
        if (result != handles.end()) {
            selection.add(kvp.second);
        }
    }

    return selection;
}

std::shared_ptr<handle>& handle_map::operator[](const size_t &index) {
    return map.at(index);
}

const std::shared_ptr<handle>& handle_map::operator[](const size_t &index) const {
    return map.at(index);
}

void handle_map::lock_all(const size_t &command_id) {
    for (auto handle : map) {
        handle.second->lock(command_id);
    }
}
