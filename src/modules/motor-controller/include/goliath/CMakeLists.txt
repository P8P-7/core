set(RELATIVE_INCLUDES
    motor-controller.h
    motor-controller/motor_controller.h
    motor-controller/motor_handle.h
    )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
    ${ABSOLUTE_INCLUDES}
    PARENT_SCOPE
    )
