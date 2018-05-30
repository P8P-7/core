set(RELATIVE_INCLUDES
        i2c.h
        i2c/handles/i2c_bus_handle.h
        i2c/handles/i2c_slave_handle.h
        i2c/i2c_error.h
        i2c/i2c_slave.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
    ${ABSOLUTE_INCLUDES}
    PARENT_SCOPE
    )
