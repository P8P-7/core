set(RELATIVE_INCLUDES
        vision.h
        vision/detectors/color_region_detector.h
        vision/detectors/detector.h
        vision/detectors/follow_line_detector.h
        vision/detectors/within_box_detector.h
        vision/processors/canny_processor.h
        vision/processors/color_processor.h
        vision/processors/roi_processor.h
        vision/util/vision_error.h
        vision/util/vision_utilities.h
        vision/webcam.h
        vision/webcam_handle.h
        )

prepend(ASOLUTE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_INCLUDES})

set(INCLUDES
        ${ABSOLUTE_INCLUDES}
        PARENT_SCOPE
        )
