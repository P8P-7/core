set(RELATIVE_INCLUDES
        openal.h
        openal/util/openal_error.h
        openal/context.h
        openal/device.h
        openal/microphone.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
