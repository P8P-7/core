namespace goliath::i2c {
    template<typename iterator_type>
    void i2c_slave::write(iterator_type begin, iterator_type end) {
        auto temp_length = std::distance(begin, end);
        if (temp_length < std::numeric_limits<size_t>::min()
            || temp_length > std::numeric_limits<size_t>::max()) {
            throw exceptions::i2c_error(device, "Buffer too large");
        }

        auto length = (size_t)(temp_length);
        write(begin, length);
    }
}