
# The one_off directory is a place for experimentation, before putting something 
# into the project files more properly.  Each time you add a directory, that 
# directory will need:
#
#   * CMakeLists.txt
#   * some_file.c
#   * some_unit_tests.test.c
#
#  Then, you need to add your new directory here, in one_off/CMakeLists
#
#   Finally, in your new directory's  one_off/your_dir/CMakeLists:
#
#       set(YOUR_PROJECT_FILES
#               ${CMAKE_CURRENT_SOURCE_DIR}/some_unit_tests.test.c
#       )
#
#       add_executable(unit_test_runner__project ${YOUR_PROJECT_FILES})
#       target_link_libraries(unit_test_runner__project PRIVATE some_file.c)
#
#  This should be enough to compile your project, if it's not, ask friend for help 
#
