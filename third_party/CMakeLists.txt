find_package(OpenCV 3 REQUIRED)

IF(NOT ${OpenCV_Found})
    add_subdirectory(opencv)
ENDIF()

MACRO(link_opencv TARGET)
    target_link_libraries(${TARGET} ${OpenCV_LIBS})
ENDMACRO(link_opencv)