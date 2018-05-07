find_package(Doxygen ${DOXYGEN_MIN_VERSION} REQUIRED)

IF(Doxygen_FOUND)
    message(${Doxygen_FOUND})

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating Documentation"
            )
ENDIF()
