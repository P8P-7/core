#include <iostream>

#include <boost/log/trivial.hpp>
#include <goliath/gpio.h>

int main() {
    GPIO gpio;
    std::string state;

    gpio.setup(GPIO17, OUT, LOW);

    for (int i = 0; i < 100; ++i) {
        gpio.set(HIGH);
        state = "State pin17: " + std::to_string(gpio.get());
        BOOST_LOG_TRIVIAL(info) << state.c_str();
        sleep(2);

        gpio.set(LOW);
        state = "State pin17: " + std::to_string(gpio.get());
        BOOST_LOG_TRIVIAL(info) << state.c_str();
        sleep(2);
    }

    gpio.close();

    return 0;
}