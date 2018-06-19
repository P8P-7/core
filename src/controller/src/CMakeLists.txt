set(RELATIVE_SOURCES
        repositories/battery_repository.cpp
        repositories/command_status_repository.cpp
        repositories/config_repository.cpp
        repositories/log_repository.cpp
        repositories/system_status_repository.cpp
        commands/command.cpp
        commands/basic_command.cpp
        commands/queue_command.cpp
        commands/move_command.cpp
        commands/wunderhorn_command.cpp
        commands/move_wing_command.cpp
        commands/dance_command.cpp
        commands/enter_command.cpp
        commands/interrupt_command_command.cpp
        commands/invalidate_all_command.cpp
        commands/line_dance_command.cpp
        commands/obstacle_course_command.cpp
        commands/synchronize_commands_command.cpp
        commands/synchronize_system_status_command.cpp
        commands/transport_rebuild_command.cpp
        commands/shutdown_command.cpp
        command_map.cpp
        command_item.cpp
        command_executor.cpp
        handle_map.cpp
        watcher.cpp
        )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
    ${ABSOLUTE_SOURCES}
    PARENT_SCOPE
    )
