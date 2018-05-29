#include <goliath/controller/handle_map.h>

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

    for (auto &key : handles) {
        auto result = map.find(key);
        if (result == map.end()) {
            throw std::runtime_error("Missing handle " + std::to_string(key));
        }
        selection.add<handles::Handle>(result->second);
    }

    return selection;
}

std::shared_ptr<Handle> &HandleMap::operator[](const size_t &index) {
    return map.at(index);
}

const std::shared_ptr<Handle> &HandleMap::operator[](const size_t &index) const {
    return map.at(index);
}

void HandleMap::lockAll(const size_t &commandId) {
    for (auto handle : map) {
        handle.second->lock(commandId);
    }
}

void HandleMap::unlockAll() {
    for (auto handle : map) {
        if(handle.second->isLocked()) {
            handle.second->unlock();
        }
    }
}

std::map<size_t, std::shared_ptr<Handle>>::iterator HandleMap::begin() {
    return map.begin();
}

std::map<size_t, std::shared_ptr<Handle>>::iterator HandleMap::end() {
    return map.end();
}
