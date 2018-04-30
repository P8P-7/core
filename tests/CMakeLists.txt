include_directories(${Boost_INCLUDE_DIR})

add_executable(test_test test_test.cpp)
add_test(test_test test_test)
target_link_libraries(test_test ${Boost_LIBRARY} ${Boost_LIBS})