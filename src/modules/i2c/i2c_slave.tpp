#include "i2c_error.h"

namespace goliath::i2c {
    template<typename IteratorType>
    void I2cSlave::write(IteratorType begin, IteratorType end) {
        auto tempLength = std::distance(begin, end) * sizeof(typename std::iterator_traits<IteratorType>::value_type);
        if (tempLength < std::numeric_limits<size_t>::min()
            || tempLength > std::numeric_limits<size_t>::max()) {
            throw exceptions::I2cError(device, "Buffer too large");
        }

        auto length = static_cast<size_t>(tempLength);
        write(reinterpret_cast<const char *>(&*begin), length);
    }
}
