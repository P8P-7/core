set(RELATIVE_SOURCES
        gpio.cpp
        gpio_handle.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
