#include "handle_map.h"

#include <map>
#include <vector>

using namespace goliath::handles;

HandleMap::HandleMap() {}
HandleMap::HandleMap(const std::map<size_t, std::shared_ptr<Handle>> &map)
    : map(map) {
}

HandleMap::~HandleMap() {
    for (auto kvp : map) {
        if (kvp.second->isLocked()) {
            kvp.second->unlock();
        }
    }
}

HandleMap HandleMap::getHandles(const std::vector<size_t> &handles) const {
    HandleMap selection{};

    for (auto kvp : map) {
        auto result = std::find(handles.begin(), handles.end(), kvp.first);
        if (result != handles.end()) {
            selection.add(kvp.second);
        }
    }

    return selection;
}

std::shared_ptr<Handle>& HandleMap::operator[](const size_t &index) {
    return map.at(index);
}

const std::shared_ptr<Handle>& HandleMap::operator[](const size_t &index) const {
    return map.at(index);
}

void HandleMap::lockAll(const size_t &command_id) {
    for (auto handle : map) {
        handle.second->lock(command_id);
    }
}
