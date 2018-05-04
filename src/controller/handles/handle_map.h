#pragma once

#include <map>

#include "handle_mutex.h"

namespace goliath::handles {
    class handle_map {
    public:
        handle_map(std::shared_ptr<std::condition_variable> var);

        std::shared_ptr<handle_mutex> get(const size_t index) const;
        std::shared_ptr<handle_mutex> operator[](const size_t index) const;

    private:
        mutable std::map<int, handle_mutex> map;
    };
}