#pragma once

#include <goliath/core.h>

/**
 * @file handle_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    // TODO: add iterator
    class HandleMap {
    public:
        HandleMap();
        explicit HandleMap(const std::map<size_t, std::shared_ptr<Handle>> &map);
        ~HandleMap();

        void add(const size_t& index, std::shared_ptr<Handle> handle);

        HandleMap getHandles(const std::vector<size_t> &handles) const;

        void lockAll(const size_t &command_id);

        std::shared_ptr<Handle>& operator[](const size_t &index);
        const std::shared_ptr<Handle>& operator[](const size_t &index) const;
    private:
        std::map<size_t, std::shared_ptr<Handle>> map;
    };
}
