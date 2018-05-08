IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
    project(goliath-documentation LANGUAGES NONE)

    IF(${CMAKE_PROJECT_NAME} MATCHES ${PROJECT_NAME})
        cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
    ENDIF()

    find_package(Doxygen ${DOXYGEN_MIN_VERSION} REQUIRED)

    IF(Doxygen_FOUND)
        set(GOLIATH_DOCUMENTATION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

        add_custom_target(${PROJECT_NAME} ALL
                ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating Documentation"
                )
    ENDIF()
ENDIF()