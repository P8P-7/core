#pragma once

#include <goliath/foundation.h>

/**
 * @file handle_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::HandleMap
     * @brief Map of all handles mapped to an integer
     */
    class HandleMap {
    public:
        using iterator = std::map<size_t, std::shared_ptr<Handle>>::iterator;
        using const_iterator = std::map<size_t, std::shared_ptr<Handle>>::const_iterator;

        HandleMap();
        /**
         * @param map Starting map
         */
        explicit HandleMap(const std::map<size_t, std::shared_ptr<Handle>> &map);
        HandleMap(const HandleMap &other);
        ~HandleMap();

        /**
         * @brief Add a handle pair
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
        /**
         * @brief Unlock all handles associated with a speciifc command
         */
        void unlockAll();

        std::shared_ptr<Handle>& operator[](const size_t &index);
        const std::shared_ptr<Handle>& operator[](const size_t &index) const;

        /**
         * @brief Begin iterator
         * @return Map iterator
         */
        iterator begin();
        /**
         * @brief End iterator
         * @return Map iterator
         */
        iterator end();

        /**
         * @brief Begin iterator
         * @return Constant map iterator
         */
        const_iterator begin() const;
        /**
         * @brief End constant iterator
         * @return Constant map iterator
         */
        const_iterator end() const;

        template<typename HandleType>
        std::shared_ptr<HandleType> get(const size_t &index) const;
    private:
        /**
         * @brief The map itself
         */
        std::map<size_t, std::shared_ptr<Handle>> map;
    };

    template<typename HandleType>
    std::shared_ptr<HandleType> HandleMap::get(const size_t &index) const {
        return std::static_pointer_cast<HandleType>(map.at(index));
    }

    template<typename HandleType>
    std::shared_ptr<HandleType> HandleMap::add(std::shared_ptr<HandleType> handle) {
        map[handle->getId()] = handle;

        return handle;
    }

    template<typename HandleType, typename ...HandleArgs>
    std::shared_ptr<HandleType> HandleMap::add(const size_t &index, HandleArgs... args) {
        auto handle = std::make_shared<HandleType>(index, args...);
        map[index] = handle;

        return handle;
    }
}
