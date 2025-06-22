cmake_minimum_required(VERSION 3.31)

# name of the project at large
project(paciFIST VERSION 0.1 LANGUAGES C)

# Generate files used in compilation ----------------------------------------------------------------------------------

# todo: run this script when setting up cmake, to prepare for a build
#find_package(Python3 REQUIRED)
#execute_process(
#        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/py_tools/generateTypesHeader/generateTypesHeader.py
#        RESULT_VARIABLE _return_code
#        #OUTPUT_VARIABLE _output
#        #ERROR_VARIABLE _error
#)
#if (NOT _return_code EQUAL 0)
#    message(FATAL_ERROR "generateTypesHeader.py failed with error: ${_error}")
#else ()
#    message(STATUS "generateTypesHeader.py has rebuilt project types header")
#endif ()


# Compiler strictness settings --------------------------------------------------------------------

# we're using C17, which is just a better version of C11,
# and hoping this will be compatible with the Switch
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_LINK_WARNING_AS_ERROR ON)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# Compiler output settings ------------------------------------------------------------------------

# set the directory for exporting the shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 


# Testing related ---------------------------------------------------------------------------------
enable_testing()


# Sub-Directories ---------------------------------------------------------------------------------
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(one_off)


# Run tests after build ---------------------------------------------------------------------------
add_custom_target(run_all_unit_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS run_unit_tests_engine
)
add_dependencies(${PROJECT_NAME}_engine run_all_unit_tests)


