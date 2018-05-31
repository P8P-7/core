set(RELATIVE_SOURCES
        util/transition_error.cpp
        parallel_reel.cpp
        phase.cpp
        reel.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
