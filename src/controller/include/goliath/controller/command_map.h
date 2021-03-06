#pragma once

#include "command_item.h"
#include "commands/command.h"
#include "repositories/command_status_repository.h"

/**
 * @file command_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    /**
     * @class goliath::commands::CommandMap
     * @brief Wrapper around a map storing indexes (Command ID's) and @see goliath::commands::CommandItem
     */
    class CommandMap {
    public:
        using iterator = std::map<size_t, CommandItem>::iterator;
        using const_iterator = std::map<size_t, CommandItem>::const_iterator;

        explicit CommandMap(std::shared_ptr<repositories::CommandStatusRepository> statusRepository);

        /**
         * @param commands Initial map
         */
        CommandMap(std::shared_ptr<repositories::CommandStatusRepository> statusRepository, std::map<size_t, CommandItem> commands);

        /**
         * @brief Add a goliath::commands::CommandItem to the map
         * @param command Pointer to @see goliath::commands::Command instance
         */
        CommandItem &add(std::shared_ptr<Command> command);

        /**
         * @brief Add a goliath::commands::CommandItem to the map.
         * Creates an instance of CommandType with handleId and args passed to it as arguments. CommandType's
         * constructor should accept an commandId as its first argument.
         * @tparam CommandType type to create an Command of
         * @tparam Targs types of arguments passed to
         * @param commandId passed to the command created
         * @param args arguments passed the command in the constructor after the commandId
         * @return goliath::commands::CommandItem with
         */
        template<typename CommandType, typename ...Targs>
        CommandItem &add(size_t commandId, Targs... args);

        /**
         * @brief Get goliath::commands::CommandItem at id
         * @param id ID
         * @return @see goliath::commands::CommandItem
         */
        CommandItem &operator[](size_t id);

        const CommandItem &operator[](size_t id) const;

        /**
         * @brief Check if a goliath::commands::Command exists
         * @param id ID to check
         * @return Status
         */
        bool commandExists(size_t id) const;

        std::shared_ptr<repositories::CommandStatusRepository> &getStatusRepository();

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
         * @brief End iterator
         * @return Constant map iterator
         */
        const_iterator end() const;

    private:
        std::shared_ptr<repositories::CommandStatusRepository> statusRepository;
        std::map<size_t, CommandItem> map;
    };

    template<typename CommandType, typename ...Targs>
    CommandItem &CommandMap::add(size_t commandId, Targs... args) {
        auto command = std::make_shared<CommandType>(commandId, args...);

        return add(command);
    }
}
