#pragma once

#include "movement_service.h"

namespace goliath::caterpillar_movement {
    class caterpillar_movement : public goliath::core::movement_service {
    public:
        void move() override;
    };
}