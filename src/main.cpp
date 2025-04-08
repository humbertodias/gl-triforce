#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cmath>

const GLfloat LIGHT_POSITION[] = {0.0f, 0.0f, 10.0f, 1.0f};
const GLfloat WHITE_LIGHT[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat TRIANGLE_COLOR[] = {0.86f, 0.74f, 0.14f};
const GLfloat SIDE_COLOR[] = {0.78f, 0.59f, 0.0f};

double angle = 0.0;
const int DEPTH = -30;

void setColor(const GLfloat *color)
{
    glColor3f(color[0], color[1], color[2]);
}

void drawTriangle(float tx, float ty, float tz)
{
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glRotated(angle, 0.0f, 1.0f, 0.0f);

    // Front face
    glBegin(GL_TRIANGLES);
    setColor(TRIANGLE_COLOR);
    glNormal3f(0, 0, 1);
    glVertex3f(-5, -5, 1);
    glVertex3f(0, 5, 1);
    glVertex3f(5, -5, 1);
    glEnd();

    // Back face
    glBegin(GL_TRIANGLES);
    setColor(TRIANGLE_COLOR);
    glNormal3f(0, 0, -1);
    glVertex3f(-5, -5, -1);
    glVertex3f(0, 5, -1);
    glVertex3f(5, -5, -1);
    glEnd();

    // Side faces
    glBegin(GL_QUADS);
    setColor(SIDE_COLOR);

    // Left side
    glNormal3f(-1, 0, 0);
    glVertex3f(-5, -5, -1);
    glVertex3f(0, 5, -1);
    glVertex3f(0, 5, 1);
    glVertex3f(-5, -5, 1);

    // Right side
    glNormal3f(1, 0, 0);
    glVertex3f(5, -5, 1);
    glVertex3f(0, 5, 1);
    glVertex3f(0, 5, -1);
    glVertex3f(5, -5, -1);
    glEnd();

    glPopMatrix();
}

void setupLighting()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE_LIGHT);
}

void setupProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = static_cast<float>(width) / height;
    float fov = glm::radians(60.0f);
    float near = 0.1f;
    float far = 100.0f;

    float top = tan(fov / 2.0f) * near;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glFrustum(left, right, bottom, top, near, far);
}

void setupModelView()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

bool initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << "\n";
        return false;
    }
    return true;
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window *window = SDL_CreateWindow("Triforce",
                                          800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (!initGLEW())
        return 1;

    setupProjection(800, 600);
    glEnable(GL_DEPTH_TEST);

    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;

    bool running = true;
    bool fullscreen = false;
    bool wireframe = false;
    SDL_Event event;

    while (running)
    {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
            {
                setupProjection(event.window.data1, event.window.data2);
                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
            }

            // now we look for a keydown event
            case SDL_EVENT_KEY_DOWN:
            {
                switch (event.key.key)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_W:
                    wireframe = !wireframe;
                    if(wireframe)
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    else
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    break;
                case SDLK_F:
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(window, fullscreen);
                    break;
                default:
                    break;
                } // end of key process
            } // end of keydown
            }
        }

        clearScreen();
        setupModelView();
        setupLighting();

        drawTriangle(-5, -5, DEPTH);
        drawTriangle(5, -5, DEPTH);
        drawTriangle(0, 5, DEPTH);

        angle = fmod(angle + 1.0, 360.0);
        SDL_GL_SwapWindow(window);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
