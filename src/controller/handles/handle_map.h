#pragma once

#include <map>

#include "handle_mutex.h"

/**
 * @file handle_map.h
 * @brief A map of all handles
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    class handle_map {
    public:
        handle_map();

        std::shared_ptr<handle_mutex> get(const size_t index) const;
        std::shared_ptr<handle_mutex> operator[](const size_t index) const;

    private:
        mutable std::map<int, std::shared_ptr<handle_mutex>> map;
    };
}