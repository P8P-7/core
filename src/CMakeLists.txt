add_subdirectory(controller)
add_subdirectory(modules)

configure_file(resources/logo.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logo.txt COPYONLY)
