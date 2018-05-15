#pragma once

/**
 * @file vision.h
 * @author Group 7 - Informatica
 */

/**
 * @namespace goliath::vision
 * @brief Module for all things vision-related
 */

#include "../../webcam.h"
#include "../../webcam_handle.h"
#include "../../util/vision_error.h"
#include "../../processors/canny_processor.h"
#include "../../processors/color_processor.h"
#include "../../detectors/detector.h"
#include "../../detectors/line_detector.h"
#include "../../detectors/follow_line_detector.h"
