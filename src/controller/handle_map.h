#pragma once

#include <map>
#include <vector>

#include <goliath/core.h>

/**
 * @file handle_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    // TODO: add iterator
    class handle_map {
    public:
        handle_map();
        explicit handle_map(const std::map<size_t, std::shared_ptr<handle>> &map);
        ~handle_map();

        template<typename HandleType>
        std::shared_ptr<HandleType> add(std::shared_ptr<HandleType> handle);
        template<typename HandleType, typename... Targs>
        std::shared_ptr<HandleType> add(const size_t &index, Targs... args);

        handle_map get_handles(const std::vector<size_t> &handles) const;

        void lock_all(const size_t &command_id);

        std::shared_ptr<handle>& operator[](const size_t &index);
        const std::shared_ptr<handle>& operator[](const size_t &index) const;
    private:
        std::map<size_t, std::shared_ptr<handle>> map;
    };

    template<typename HandleType>
    std::shared_ptr<HandleType> handle_map::add(std::shared_ptr<HandleType> handle) {
        map[handle->get_handle_id()] = handle;
    }

    template<typename HandleType, typename... HandleArgs>
    std::shared_ptr<HandleType> handle_map::add(const size_t &index, HandleArgs... args) {
        auto handle = std::make_shared<HandleType>(index, args...);
        map[index] = handle;

        return handle;
    }
}
