
# the "Vendor" directory, holds all of the third party software that we're using.
# TODO: imgui, ..., ?

# sets up an external linking library for SDL
# all SDL things should be linked into sdl_lib
add_subdirectory(SDL)
add_library(sdl_lib INTERFACE)
target_link_libraries(sdl_lib INTERFACE SDL3::SDL3)

# SDL_image - libpng, etc
add_subdirectory(SDL_image)
add_library(sdl_image_lib INTERFACE)
target_link_libraries(sdl_image_lib INTERFACE SDL3_image::SDL3_image)

# SDL_mixer - good audio
add_subdirectory(SDL_mixer)
add_library(sdl_mixer_lib INTERFACE)
target_link_libraries(sdl_mixer_lib INTERFACE SDL3_mixer::SDL3_mixer)

# SDL_tff - fonts
add_subdirectory(SDL_ttf)
add_library(sdl_ttf_lib INTERFACE)
target_link_libraries(sdl_ttf_lib INTERFACE SDL3_ttf::SDL3_ttf)

# check unit test framework
add_subdirectory(check)
add_library(check_lib INTERFACE)
target_link_libraries(check_lib INTERFACE check)
