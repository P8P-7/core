set(RELATIVE_SOURCES
        commands/command.cpp
        commands/basic_command.cpp
        commands/queue_command.cpp
        commands/move_command.cpp
        commands/wunderhorn_command.cpp
        commands/move_tower_command.cpp
        commands/move_wing_command.cpp
        commands/dance_command.cpp
        commands/invalidate_all_command.cpp
        commands/line_dance_command.cpp
        commands/obstacle_course_command.cpp
        commands/transport_rebuild_command.cpp
        command_map.cpp
        command_executor.cpp
        handle_map.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
    ${ABSOLUTE_SOURCES}
    PARENT_SCOPE
    )
