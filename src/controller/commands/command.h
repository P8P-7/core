#pragma once

#include <vector>
#include "../handle_map.h"

namespace goliath::commands {
    class command {
    public:
        explicit command(const std::vector<size_t> &required_handles);

        void run(const handles::handle_map &handles);

        void interrupt();
        bool is_interrupted() const;

        const std::vector<size_t>& get_required_handles() const;

    protected:
        virtual void execute(const handles::handle_map &handles) = 0;

    private:
        bool interrupted = false;
        bool running = false;
        const std::vector<size_t> required_handles;
    };
}