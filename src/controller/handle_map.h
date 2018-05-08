#pragma once

#include <map>
#include <vector>

#include <goliath/core.h>

/**
 * @file handle_map.h
 * @brief A map of all handles
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    // TODO: add iterator
    class handle_map {
    public:
        handle_map();
        explicit handle_map(const std::map<size_t, std::shared_ptr<handle>> &map);
        ~handle_map();

        void add(const size_t& index, std::shared_ptr<handle> handle);

        handle_map get_handles(const std::vector<size_t> &handles) const;

        void lock_all(const size_t &command_id);

        std::shared_ptr<handle>& operator[](const size_t &index);
        const std::shared_ptr<handle>& operator[](const size_t &index) const;
    private:
        std::map<size_t, std::shared_ptr<handle>> map;
    };
}