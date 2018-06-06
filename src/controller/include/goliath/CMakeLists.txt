set(RELATIVE_INCLUDES
        controller.h
        controller/repositories/battery_repository.h
        controller/repositories/command_status_repository.h
        controller/repositories/config_repository.h
        controller/repositories/log_repository.h
        controller/commands/command.h
        controller/commands/dance_command.h
        controller/commands/invalidate_all_command.h
        controller/commands/line_dance_command.h
        controller/commands/move_command.h
        controller/commands/move_wing_command.h
        controller/commands/obstacle_course_command.h
        controller/commands/transport_rebuild_command.h
        controller/commands/wunderhorn_command.h
        controller/command_executor.h
        controller/command_map.h
        controller/handle_map.h
        controller/handles.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
