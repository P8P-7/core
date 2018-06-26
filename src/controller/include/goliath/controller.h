#pragma once

/**
 * @file controller.h
 * @author Group 7 - Informatica
 */

#include "controller/repositories/config_repository.h"
#include "controller/repositories/command_status_repository.h"
#include "controller/repositories/battery_repository.h"
#include "controller/repositories/log_repository.h"
#include "controller/repositories/system_status_repository.h"
#include "controller/watcher.h"
#include "controller/commands/command.h"
#include "controller/commands/basic_command.h"
#include "controller/commands/queue_command.h"
#include "controller/commands/wunderhorn_command.h"
#include "controller/commands/move_command.h"
#include "controller/commands/move_wing_command.h"
#include "controller/commands/move_arm_command.h"
#include "controller/commands/grip_command.h"
#include "controller/commands/dance_command.h"
#include "controller/commands/enter_command.h"
#include "controller/commands/interrupt_command_command.h"
#include "controller/commands/invalidate_all_command.h"
#include "controller/commands/line_dance_command.h"
#include "controller/commands/obstacle_course_command.h"
#include "controller/commands/synchronize_commands_command.h"
#include "controller/commands/synchronize_system_status_command.h"
#include "controller/commands/synchronize_battery_voltage_command.h"
#include "controller/commands/transport_rebuild_command.h"
#include "controller/commands/shutdown_command.h"
#include "controller/commands/set_wing_position_command.h"
#include "controller/command_executor.h"
#include "controller/command_item.h"
#include "controller/command_map.h"
#include "controller/handles.h"
#include "controller/handle_map.h"
