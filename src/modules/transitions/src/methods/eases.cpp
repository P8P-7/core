#include <goliath/transitions/methods/eases.h>

double goliath::transitions::methods::easeInQuad(double value) {
    return value * value;
}

double goliath::transitions::methods::easeOutQuad(double value) {
    return value * (2 - value);
}

double goliath::transitions::methods::easeInOutQuad(double value) {
    return value < 0.5 ? 2 * value * value : -1 + (4 - 2 * value) * value;
}

double goliath::transitions::methods::easeInCubic(double value) {
    return value * value * value;
}

double goliath::transitions::methods::easeOutCubic(double value) {
    return (value - 1) * value * value + 1;
}

double goliath::transitions::methods::easeInOutCubic(double value) {
    return value < 0.5 ? 4 * value * value * value : (value - 1) * (2 * value - 2) * (2 * value - 2) + 1;
}

double goliath::transitions::methods::easeInQuart(double value) {
    return value * value * value * value;
}

double goliath::transitions::methods::easeOutQuart(double value) {
    return 1 - (value - 1) * value * value * value;
}

double goliath::transitions::methods::easeInOutQuart(double value) {
    return value < 0.5 ? 8 * value * value * value * value : 1 - 8 * (value - 1) * value * value * value;
}

double goliath::transitions::methods::easeInQuint(double value) {
    return value * value * value * value * value;
}

double goliath::transitions::methods::easeOutQuint(double value) {
    return 1 + (value - 1) * value * value * value * value;
}

double goliath::transitions::methods::easeInOutQuint(double value) {
    return value < 0.5 ? 16 * value * value * value * value * value : 1 +
                                                                      16 * (value - 1) * value * value * value * value;
}
