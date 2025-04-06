#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_keycode.h>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLFunctions.h"

void SDLErrorExit(const std::string &_msg) {
    std::cerr << _msg << std::endl;
    std::cerr << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
}

SDL_GLContext createOpenGLContext(SDL_Window *window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    return SDL_GL_CreateContext(window);
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDLErrorExit("Unable to initialize SDL");
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 OpenGL4.x Triangle",
                                          1024, 720,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!window) {
        SDLErrorExit("Unable to create window");
    }

    SDL_GLContext glContext = createOpenGLContext(window);
    if (!glContext) {
        SDLErrorExit("Problem creating OpenGL context");
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
#endif

    auto vaoID = createTriangle(0.8f);
    const std::string vertex = R"(
#version 400 core
layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inColour;
layout (location = 2) in vec3 inNormal;
uniform mat4 MVP;
uniform mat4 model;
out vec3 vertColour;
out vec3 normal;
out vec3 fragPos;
void main() {
    gl_Position = MVP * vec4(inPosition, 1.0);
    vertColour = inColour;
    normal = normalize(inNormal);
    fragPos = vec3(model * vec4(inPosition, 1.0));
})";

    const std::string fragment = R"(
#version 400 core
in vec3 vertColour;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColour;
uniform vec3 lightPos;
void main() {
    vec3 s = normalize(lightPos - fragPos);
    vec3 v = normalize(-fragPos);
    vec3 h = normalize(v + s);
    float sDotN = max(dot(s, normal), 0.0);
    vec3 diffuse = vertColour * sDotN;
    fragColour = vec4(diffuse, 1.0);
})";

    auto shaderID = loadShaderFromStrings(vertex, fragment);
    auto MVP = glGetUniformLocation(shaderID, "MVP");
    auto model = glGetUniformLocation(shaderID, "model");
    auto lightPos = glGetUniformLocation(shaderID, "lightPos");

    glUniform3f(lightPos, 0.0f, 2.0f, 0.0f);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    bool quit = false;
    float rotation = 0.0f;
    SDL_Event event;

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 3.0f, 3.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 project = glm::perspective(45.0f, 1024.0f / 720.0f, 0.01f, 200.0f);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_WINDOW_RESIZED: {
                    int w, h;
                    SDL_GetWindowSizeInPixels(window, &w, &h);
                    glViewport(0, 0, w, h);
                    project = glm::perspective(45.0f, static_cast<float>(w) / h, 0.01f, 200.0f);
                    break;
                }

                // now we look for a keydown event
                case SDL_EVENT_KEY_DOWN: {
                    switch (event.key.key) {
                        case SDLK_ESCAPE:
                            break;
                        case SDLK_W:
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            break;
                        case SDLK_G:
                            SDL_SetWindowFullscreen(window, false);
                            break;
                        default:
                            break;
                    } // end of key process
                } // end of keydown
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rotation += 1.0f;
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        auto transform = project * view * rotY;
        glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(view * rotY));

        glBindVertexArray(vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    }

    glDeleteProgram(shaderID);
    glDeleteVertexArrays(1, &vaoID);
    SDL_Quit();

    return EXIT_SUCCESS;
}
