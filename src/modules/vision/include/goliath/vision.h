#pragma once

/**
 * @file vision.h
 * @author Group 7 - Informatica
 */

/**
 * @namespace goliath::vision
 * @brief Module for all things vision-related
 */

#include "vision/webcam.h"
#include "vision/webcam_handle.h"
#include "vision/util/vision_error.h"
#include "vision/processors/canny_processor.h"
#include "vision/processors/color_processor.h"
#include "vision/processors/roi_processor.h"
#include "vision/detectors/detector.h"
#include "vision/detectors/line_detector.h"
#include "vision/detectors/follow_line_detector.h"
#include "vision/detectors/color_region_detector.h"
