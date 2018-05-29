set(RELATIVE_INCLUDES
        vision.h
        servo/servo_handle.h
        )

prepend(ASOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
