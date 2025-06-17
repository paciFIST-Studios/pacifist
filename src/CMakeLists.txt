# paciFIST studios. 2025. MIT License

# NOTE: I think we either have to manually include everything, or write a script 
# that will re-create this file dynamically.  For now (120250615), I'm going to 
# require manual editing.  Yes it is tedious, yes it does constrain development time.  
#
# The goal here, is to help force the project towards:
#       * smallness
#       * simplicity
#       * __needed__ changes
#
#
# Instructions:
#
#   If you need to add a file so it's compiled into the project, then:
#       1. include the file as a "source" if:
#           a. it's a .c file
#           b. it's a "header only" file
#
#       2. include the file as "include", if it's a regular header
#

# here, we're setting a local variable, ENGINE_LIB_SOURCE_FILES
# and we're using it to hold ALL of the files we're compiling.
# so this is 100% manual.  If something is "header only", it 
# goes in this list too
# TODO: Update so we automatically get files, but we need an "allow list" and a "block list", and for something to be included it needs to be on the "allow list" and NOT on the "block list":w
set(ENGINE_LIB_SOURCE_FILES
        
        # core 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/WindowConfigStruct.h
        # log
        ${CMAKE_CURRENT_SOURCE_DIR}/log/log_utility.h
        # memory
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/LinkedList.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/LinkedList.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/MemoryArena.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/RedBlackTree.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/RedBlackTree.c
        # os
        ${CMAKE_CURRENT_SOURCE_DIR}/os/path.h
        # parsers
        ${CMAKE_CURRENT_SOURCE_DIR}/parsers/ConfigFileParser.h
)

# "proper" headers go here
set(ENGINE_LIB_INCLUDE_FILES

)

# all the dependencies the engine library uses go here
# we might add like, bullet physics, imgui, some kind of
# voice chat, etc
set(ENGINE_LIB_DEPENDENCIES
        sdl_lib
        sdl_image_lib
        sdl_mixer_lib
        sdl_ttf_lib
)



# Engine Library ----------------------------------------------------------------------------------

# the paciFIST_engine_library is a lib that contains all of the "engine"
# and game code.  The application will link against this library.
# We're doing this, so the game is easier to test.  The unit tests
# use "Check", and create a binary of their own.  We need this lib
# to exist, so our unit test binary can link against it.

add_library(${PROJECT_NAME}_engine_lib STATIC ${ENGINE_LIB_SOURCE_FILES})
set_target_properties(${PROJECT_NAME}_engine_lib PROPERTIES LINKER_LANGUAGE C)
target_sources(${PROJECT_NAME}_engine_lib PRIVATE ${ENGINE_LIB_SOURCE_FILES})
target_include_directories(${PROJECT_NAME}_engine_lib PRIVATE ${ENGINE_INCLUDE_LIB_FILE})
target_link_libraries(${PROJECT_NAME}_engine_lib PUBLIC ${ENGINE_LIB_DEPENDENCIES})



# Engine executable -------------------------------------------------------------------------------

# the engine Executable is the Engine Library, plus the main.c file.
# Part of the reason for doing this, is to have some abstraction between
# the "platform"/"hardware" layer, and the "game" layer.

add_executable(${PROJECT_NAME}_engine)
set_target_properties(${PROJECT_NAME}_engine PROPERTIES LINKER_LANGUAGE C)
target_sources(${PROJECT_NAME}_engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        ${ENGINE_LIB_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}_engine PRIVATE ${ENGINE_LIB_INCLUDE_FILES})
target_link_libraries(${PROJECT_NAME}_engine PUBLIC ${ENGINE_LIB_DEPENDENCIES})


