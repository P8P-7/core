#pragma once

#include <vector>

#include <Message.pb.h>

/**
 * @file command.h
 * @brief Command base class
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class command {
    public:
        explicit command(const std::vector<unsigned> &occupying_handles);

        virtual void execute(Message &message) = 0;

        void interrupt();
        const std::vector<unsigned>& get_handles() const;

    protected:
        bool interrupted = false;

        const std::vector<unsigned> occupying_handles;
    };
}