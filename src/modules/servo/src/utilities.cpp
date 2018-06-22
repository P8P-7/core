#include <goliath/servo/utilities.h>

using namespace goliath;
using namespace goliath::servo;

Direction Utilities::flipDirection(Direction direction) {
    return direction == Direction::CLOCKWISE ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
}
