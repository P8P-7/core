find_package(OpenCV 3.1.4 REQUIRED)

IF(NOT OpenCV_FOUND)
    add_subdirectory(opencv)
ENDIF()

MACRO(link_opencv TARGET)
    IF(OpenCV_FOUND)
        target_link_libraries(${TARGET} ${OpenCV_LIBS})
    ENDIF()
ENDMACRO(link_opencv)

find_package(cppzmq REQUIRED)

IF(NOT cppzmq_FOUND)
    add_subdirectory(cppzmq)
ENDIF()

MACRO(link_cppzmq TARGET)
    IF(cppzmq_FOUND)
        include_directories(${cppzmq_INCLUDE_DIR})
        target_link_libraries(${TARGET} ${cppzmq_LIBRARY})
    ENDIF()
ENDMACRO()