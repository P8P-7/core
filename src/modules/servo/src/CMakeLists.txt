set(RELATIVE_SOURCES
        utilities.cpp
        servo_handle.cpp
        wing_handle.cpp
        wings/wing_controller.cpp
        wings/wing_state_repository.cpp
        wings/wing_state.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
