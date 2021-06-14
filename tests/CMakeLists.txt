enable_testing()

add_executable(test_utils test_utils.c)
target_link_libraries(test_utils utils)

add_test(NAME test_utils 
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMAND test_utils)
