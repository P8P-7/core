set(RELATIVE_SOURCES
    detectors/detector.cpp
    detectors/line_detector.cpp
    detectors/within_box_detector.cpp
    detectors/follow_line_detector.cpp
    detectors/color_region_detector.cpp
    processors/processor.cpp
    processors/color_processor.cpp
    processors/canny_processor.cpp
    processors/roi_processor.cpp
    util/vision_error.cpp
    util/vision_utilities.cpp
    webcam_handle.cpp
    webcam.cpp
    )

prepend(ABSOLUTE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} ${RELATIVE_SOURCES})

set(SOURCES
    ${ABSOLUTE_SOURCES}
    PARENT_SCOPE
    )
