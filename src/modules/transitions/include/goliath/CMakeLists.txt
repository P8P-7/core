set(RELATIVE_INCLUDES
        transitions.h
        transitions/methods/eases.h
        transitions/util/transition_error.h
        transitions/util/transition_utilities.h
        transitions/parallel_reel.h
        transitions/phase.h
        transitions/reel.h
        transitions/transition_executor.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
