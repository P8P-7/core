set(RELATIVE_SOURCES
        handles/i2c_bus_handle.cpp
        handles/i2c_slave_handle.cpp
        i2c_error.cpp
        i2c_slave.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
