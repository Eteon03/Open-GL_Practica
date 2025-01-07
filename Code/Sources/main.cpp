
// Este código es de dominio público
// angel.rodriguez@udit.es

#include <iostream>
#include <SDL.h>
#include "../Headers/Scene.hpp"
#include "../Headers/Camera.hpp"
#include <Window.hpp>

using udit::Scene;
using udit::Window;

int main(int, char* [])
{
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Window window
    (
        "OpenGL example",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height,
        { 3, 3 }
    );

    Scene scene(viewport_width, viewport_height);

    bool exit = false;
    Uint32 last_time = SDL_GetTicks();

    do
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                scene.process_mouse_motion(event.motion.xrel, -event.motion.yrel);
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        scene.process_input(keystate, delta_time);
        scene.update();
        scene.render();

        window.swap_buffers();
    } while (!exit);

    SDL_Quit();

    return 0;
}


