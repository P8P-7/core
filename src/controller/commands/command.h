#pragma once

#include <vector>

namespace goliath::commands {
    class command {
    public:
        explicit command(const std::vector<unsigned> &occupying_handles);

        virtual void execute() = 0;

        void interrupt();
        const std::vector<unsigned>& get_handles() const;

    protected:
        bool interrupted = false;

        const std::vector<unsigned> occupying_handles;
    };
}