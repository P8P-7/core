set(RELATIVE_INCLUDES
        transitions.h
        transitions/util/transition_error.h
        transitions/util/transition_utilities.h
        transitions/phase.h
        transitions/phase.tpp
        transitions/reel.h
        )

prepend(ASOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
