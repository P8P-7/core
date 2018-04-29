#pragma once

#include <map>

#include "handle_mutex.h"

namespace goliath::handles {
    class handle_map {
    public:
        handle_map(std::shared_ptr<std::condition_variable> var);

        handle_mutex *operator[](const int index);

    private:
        std::map<int, handle_mutex> map;
    };
}