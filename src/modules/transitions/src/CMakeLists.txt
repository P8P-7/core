set(RELATIVE_SOURCES
        methods/eases.cpp
        util/transition_error.cpp
        parallel_reel.cpp
        phase.cpp
        reel.cpp
        tickable.cpp
        transition_executor.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
