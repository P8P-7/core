#pragma once

#include <vector>

namespace goliath::commands {
    class Command {
    public:
        Command() {}

        Command(const std::vector<unsigned> &oh)
            : occupying_handles(oh) {}

        virtual void execute() = 0;

        void interrupt();
        const std::vector<unsigned>& get_handles() const;

    protected:
        bool interrupted = false;

        const std::vector<unsigned> occupying_handles;
    };
}