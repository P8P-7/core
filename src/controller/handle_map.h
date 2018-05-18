#pragma once

#include <goliath/foundation.h>

/**
 * @file handle_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    // TODO: add iterator
    /**
     * @class goliath::handles::HandleMap
     * @brief Map of all handles mapped to an integer
     */
    class HandleMap {
    public:
        HandleMap();
        /**
         * @param map Starting map
         */
        explicit HandleMap(const std::map<size_t, std::shared_ptr<Handle>> &map);
        ~HandleMap();

        /**
         * @brief Add a handle pair
         * @param index New index of handle
         * @param handle Pointer to the handle itself
         */
        template<typename HandleType>
        std::shared_ptr<HandleType> add(std::shared_ptr<HandleType> handle);
        template<typename HandleType, typename... Targs>
        std::shared_ptr<HandleType> add(const size_t &index, Targs... args);

        /**
         * @brief Get handles for a specific selecion of ID's
         * @param handles Vector of ID's you want to gather
         * @return HandleMap Selection
         */
        HandleMap getHandles(const std::vector<size_t> &handles) const;

        /**
         * @brief Lock all handles for a specific command
         * @param commandId Command ID for the handle's to be locked on
         */
        void lockAll(const size_t &commandId);

        std::shared_ptr<Handle>& operator[](const size_t &index);
        const std::shared_ptr<Handle>& operator[](const size_t &index) const;
    private:
        /**
         * @brief The map itself
         */
        std::map<size_t, std::shared_ptr<Handle>> map;
    };

    template<typename HandleType>
    std::shared_ptr<HandleType> HandleMap::add(std::shared_ptr<HandleType> handle) {
        map[handle->getHandleId()] = handle;
    }

    template<typename HandleType, typename... HandleArgs>
    std::shared_ptr<HandleType> HandleMap::add(const size_t &index, HandleArgs... args) {
        auto handle = std::make_shared<HandleType>(index, args...);
        map[index] = handle;

        return handle;
    }
}
