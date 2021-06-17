# Utilities
add_library(utils STATIC batch_exec__utils.c batch_exec__loops.c)
target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(utils PUBLIC _DEFAULT_SOURCE) #  FILE* popen(...)
target_link_libraries(utils PUBLIC MPI::MPI_CXX)

# Main program
add_executable(batch_exec batch_exec.c)
target_link_libraries(batch_exec utils)

install(TARGETS batch_exec 
        RUNTIME DESTINATION bin)
