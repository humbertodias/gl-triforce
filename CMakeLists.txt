cmake_minimum_required(VERSION 3.14)
project(triforce)

set (CMAKE_CXX_STANDARD 14)

# Dependencies managed by vcpkg (toolchain must be passed via command line)
find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Define the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Global compile definitions
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Platform-specific settings
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # macOS: silence OpenGL deprecation warnings, link Metal framework
    add_definitions(-DGL_SILENCE_DEPRECATION)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Metal")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux: link OpenGL manually
    target_link_libraries(${PROJECT_NAME} PRIVATE -lGL)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows: enable math definitions and suppress common warnings
    add_compile_definitions(_USE_MATH_DEFINES NOMINMAX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE /wd4251)
endif()

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glm::glm
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

# Include local project headers
target_include_directories(${PROJECT_NAME} PRIVATE inc)
