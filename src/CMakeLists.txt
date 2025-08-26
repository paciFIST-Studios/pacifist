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

# only the src project is being compiled with these warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic")
# set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fsanitize=address")




# here, we're setting a local variable, ENGINE_LIB_SOURCE_FILES
# and we're using it to hold ALL of the files we're compiling.
# so this is 100% manual.  If something is "header only", it 
# goes in this list too
# TODO: Update so we automatically get files, but we need an "allow list" and a "block list", and for something to be included it needs to be on the "allow list" and NOT on the "block list":w
set(ENGINE_LIB_SOURCE_FILES
        # _games/littlest_necromancer
        ${CMAKE_CURRENT_SOURCE_DIR}/_games/LittlestNecromancer/LittlestNecromancer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/_games/LittlestNecromancer/LNGameMode.h
        # _games/snake
        ${CMAKE_CURRENT_SOURCE_DIR}/_games/snake.h


        # algo
        ${CMAKE_CURRENT_SOURCE_DIR}/algo/search.h
        ${CMAKE_CURRENT_SOURCE_DIR}/algo/search.c
        # bit
        ${CMAKE_CURRENT_SOURCE_DIR}/bit/bitfield.h
        # config
        ${CMAKE_CURRENT_SOURCE_DIR}/config/PFConfigurationFileParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/config/PFConfigurationParser.c
        # core 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/assert.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/debug.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/define.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/error.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/error.c
        ${CMAKE_CURRENT_SOURCE_DIR}/core/WindowConfigStruct.h
        # date
        ${CMAKE_CURRENT_SOURCE_DIR}/date/datetime.h
        ${CMAKE_CURRENT_SOURCE_DIR}/date/datetime.c
        # engine
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/PFEngineState.h
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/PFEngineState.c
        # f,g,h
        # imgui
        # j,k
        # log
        ${CMAKE_CURRENT_SOURCE_DIR}/log/log_utility.h
        ${CMAKE_CURRENT_SOURCE_DIR}/log/log.h
        ${CMAKE_CURRENT_SOURCE_DIR}/log/log.c
        ${CMAKE_CURRENT_SOURCE_DIR}/log/log_category.h
        # memory
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/type/typed_pointer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/allocator/PFAllocator.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/allocator/allocator_utilities.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/handle/PFMemoryHandle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/hash_table/QuickHashTable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/hash_table/PFCompactHashTable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/hash_table/PFCompactHashTable.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/list/LinkedList.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/list/LinkedList.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/hash_table/HashTable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/hash_table/HashTable.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/allocator/MemoryArena.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/allocator/PFAllocator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/tree/RedBlackTree.h
        ${CMAKE_CURRENT_SOURCE_DIR}/memory/tree/RedBlackTree.c
        # n
        # os
        ${CMAKE_CURRENT_SOURCE_DIR}/os/path.h
        ${CMAKE_CURRENT_SOURCE_DIR}/os/os_utility.h
        ${CMAKE_CURRENT_SOURCE_DIR}/os/os_utility.c
        # project data types
        ${CMAKE_CURRENT_SOURCE_DIR}/project_data_types.h
        # q
        # reflection
        ${CMAKE_CURRENT_SOURCE_DIR}/reflection/metadata_reflection_system.c
        ${CMAKE_CURRENT_SOURCE_DIR}/reflection/metadata_reflection_system.h
        # resource
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/PFResourceManager.h
        # string
        ${CMAKE_CURRENT_SOURCE_DIR}/string/PFStringInternmentSingleton.h
        ${CMAKE_CURRENT_SOURCE_DIR}/string/PFStringInternmentSingleton.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/pstring.h
        ${CMAKE_CURRENT_SOURCE_DIR}/string/pstring.c
        gui/PFDebugGui.h
        gui/PFGuiState.h
        _games/LittlestNecromancer/LittlestNecromancer.c
        # type
        # u,v,w,x,y,z 
)

set(ENGINE_LIB_INCLUDE_DIRECTORIES
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



# Doxygen -------------------------------------------------------------------------------------------------------------

option(BUILD_DOC "Build Documentation" ON)

set(DOXYGEN_GENERATE_HTML YES)

message("Doxygen build")

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build")
    
    add_custom_target( doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
else ()
    message("Doxygen needs to be installed to generate API documentation")
endif ()
