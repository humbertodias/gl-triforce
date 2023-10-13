cmake_minimum_required(VERSION 3.7)
project(triforce)

set(CMAKE_CXX_STANDARD 20)
string(APPEND CMAKE_CXX_FLAGS " -Wno-c++11-narrowing")

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
set(BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

# FetchContent_Declare(
#         sdl2
#         GIT_REPOSITORY https://github.com/libsdl-org/sdl12-compat
#         GIT_TAG        release-1.2.68
# )
FetchContent_GetProperties(sdl2)
if(NOT sdl2_POPULATED)
    FetchContent_Populate(sdl2)
    if (MSVC)
        set(BUILD_SHARED_LIBS ON)
    else()
        set(BUILD_SHARED_LIBS OFF)
    endif()
    set(SDL_X11_SHARED OFF)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.6.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

# Fix - for sdl2_image
set(SDL2_IMAGE_CMAKELISTS "_deps/sdl2_image-src/CMakeLists.txt")
file(READ "${SDL2_IMAGE_CMAKELISTS}" FILE_CONTENT)
string(REPLACE "CMAKE_SOURCE_DIR" "CMAKE_CURRENT_SOURCE_DIR" FILE_CONTENT "${FILE_CONTENT}" )
file(WRITE "${SDL2_IMAGE_CMAKELISTS}" "${FILE_CONTENT}")

# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
# END ADDITION

FetchContent_MakeAvailable(SDL2_image)



# FetchContent_Declare(
#         GLM
#         GIT_REPOSITORY https://github.com/g-truc/glm
#         GIT_TAG 0.9.9.8
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(GLM)

# message("GLM_SOURCE_DIR : ${GLM_SOURCE_DIR}")

# FetchContent_Declare(
#         GLEW
#         GIT_REPOSITORY https://github.com/edoren/glew
#         GIT_TAG v2.0.0
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(GLEW)


# include_directories(glimac/include _deps/glm-src _deps/glew-src/include)

add_executable(${PROJECT_NAME} triforce.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)

