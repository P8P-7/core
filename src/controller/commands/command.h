#pragma once

#include <vector>
#include "../handles/handle_map.h"

namespace goliath::commands {
    class command {
    public:
        explicit command(const std::vector<size_t> &required_handles);

        virtual void execute(const handles::handle_map &handles) = 0;

        void interrupt();
        bool is_interrupted() const;
        const std::vector<size_t>& get_required_handles() const;

    protected:
        bool interrupted = false;

        const std::vector<size_t> required_handles;
    };
}