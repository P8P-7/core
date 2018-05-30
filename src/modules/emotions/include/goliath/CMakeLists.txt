set(RELATIVE_INCLUDES
        emotions.h
        emotions/repositories/emotion_repository.h
        emotions/emotion_handle.h
        emotions/emotion_publisher.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
