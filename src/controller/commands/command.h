#pragma once

#include <vector>

namespace goliath::commands {
    class Command {
    public:
        Command(const std::vector<unsigned> &oh)
            : occupying_handles(oh) {}

        virtual void execute() = 0;

        const std::vector<unsigned>& get_handles();

    protected:
        const std::vector<unsigned> occupying_handles;
    };

    const std::vector<unsigned>& Command::get_handles() {
        return occupying_handles;
    }
}