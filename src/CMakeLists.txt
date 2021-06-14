# Utilities
add_library(utils STATIC batch_exec__utils.c)
target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(utils PUBLIC _DEFAULT_SOURCE) #  FILE* popen(...)

# Main program
add_executable(batch_exec batch_exec.c)
target_link_libraries(batch_exec utils MPI::MPI_CXX)