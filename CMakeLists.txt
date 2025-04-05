cmake_minimum_required(VERSION 3.14)
project(triforce)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

# sdl2
FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.32.4
)
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

# glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glm)

# glew
FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
        GIT_TAG glew-cmake-2.2.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
    FetchContent_Populate(glew)
    set(glew-cmake_BUILD_SHARED OFF)
    set(glew-cmake_BUILD_STATIC ON)
    set(ONLY_LIBS ON)
    add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_package(OpenGL REQUIRED)
if(${CMAKE_SYSTEM_NAME}  MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME}  MATCHES  "Windows")
    find_package(GLEW REQUIRED)
endif()


add_executable(${PROJECT_NAME} src/main.cpp)
include_directories(inc ${glm_SOURCE_DIR})
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

message("sdl2_SOURCE_DIR: ${sdl2_SOURCE_DIR}
glew_SOURCE_DIR: ${glew_SOURCE_DIR}
glm_SOURCE_DIR: ${glm_SOURCE_DIR}
")

# see what platform we are on and set platform defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Metal")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set ( PROJECT_LINK_LIBS -lGL)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(NOMINMAX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE /wd4251)
endif()

# add exe and link libs that must be after the other defines
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static OpenGL::GL)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME}  MATCHES  "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINK_LIBS} GLEW::GLEW glm)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC 
${sdl2_SOURCE_DIR}/include ${sdl2_BINARY_DIR}/include
${glew_SOURCE_DIR}/include ${glew_BINARY_DIR}/include
${glm_SOURCE_DIR}/include ${glm_BINARY_DIR}/include
inc
)

# Link with dependencies
# see what platform we are on and set platform defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Metal")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set ( PROJECT_LINK_LIBS -lGL)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(NOMINMAX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE /wd4251)
endif()


# add exe and link libs that must be after the other defines
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static OpenGL::GL)

if(${CMAKE_SYSTEM_NAME}  MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME}  MATCHES  "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINK_LIBS} GLEW::GLEW glm)
endif()