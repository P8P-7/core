#pragma once

namespace goliath::commands {
    class command {
        virtual void execute() = delete;
    };
}