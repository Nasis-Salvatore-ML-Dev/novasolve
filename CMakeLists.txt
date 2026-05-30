cmake_minimum_required(VERSION 3.10)

# 1. Define Project
project(NovaSolve VERSION 1.0 LANGUAGES CXX)

# 2. Force C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. HPC Optimization Flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
    message(STATUS "Configuring for High-Performance Release Mode")
else()
    add_compile_options(-g -Wall -Wextra)
    message(STATUS "Configuring for Debug Mode with Assertions")
endif()

# 4. Define the Library (Header-Only Engine)
add_library(NovaSolve INTERFACE)
target_include_directories(NovaSolve INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 5. Unit Tests
add_executable(unit_tests_vector tests/test_vector.cpp)
target_link_libraries(unit_tests_vector PRIVATE NovaSolve)

add_executable(unit_tests_matrix tests/test_matrix.cpp)
target_link_libraries(unit_tests_matrix PRIVATE NovaSolve)

# 6. Plotting / Visualization Tests
add_executable(plot_tests tests/test_plot.cpp)
target_link_libraries(plot_tests PRIVATE NovaSolve)


#7. Experiments
add_executable(exp_matrix tests/exp_matrix.cpp)
target_link_libraries(exp_matrix PRIVATE NovaSolve)

# 8. LU factorization tests
add_executable(test_lu tests/test_lu.cpp)
target_link_libraries(test_lu PRIVATE NovaSolve)

# 9. Examples
add_executable(lu_decomp examples/lu.cpp)
target_link_libraries(lu_decomp PRIVATE NovaSolve)

# 10. Sparse matrix tests
add_executable(test_sparse tests/test_sparse.cpp)
target_link_libraries(test_sparse PRIVATE NovaSolve)