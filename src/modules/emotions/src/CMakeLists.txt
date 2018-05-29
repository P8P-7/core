set(RELATIVE_SOURCES
        emotion_handle.cpp
        emotion_publisher.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
        ${ABSOLUTE_SOURCES}
        PARENT_SCOPE
        )
