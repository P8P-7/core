set(RELATIVE_INCLUDES
        servo/repositories/wing_state_repository.h
        servo/repositories/wing_state.h
        servo/wings/wing_controller.h
        servo/wings/commandbuilder/wing_command_builder.h
        servo/wings/wing_command.h
        servo/servo_handle.h
        servo/wing_handle.h
        servo/servo_mode.h
        servo/utilities.h
        servo.h
        )

prepend(ABSOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
