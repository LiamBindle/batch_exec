enable_testing()

add_executable(test_utils test_utils.c)
target_link_libraries(test_utils utils)

add_test(NAME test_utils 
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMAND test_utils)

add_executable(test_exec test_batch_exec.c)
target_link_libraries(test_exec utils)

add_test(NAME test_exec
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 6 ${CMAKE_CURRENT_BINARY_DIR}/test_exec)