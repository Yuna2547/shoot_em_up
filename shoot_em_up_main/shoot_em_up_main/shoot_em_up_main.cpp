#include <SDL3/SDL.h>

int main(int argc, char** argv)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_SetAppMetadata("SDL Test", "1.0", "games.anakata.test-sdl");
    if (!SDL_Init(SDL_INIT_VIDEO))
        return 1;

    if (!SDL_CreateWindowAndRenderer("HELLO SDL", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer))
        return 1;

    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    bool keepGoing = true;
    do
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                keepGoing = false;
        }

        const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */

        const float red = (float)(0.5 + 0.5 * SDL_sin(now));
        const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
        const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
        SDL_SetRenderDrawColorFloat(renderer, red, green, blue, 1.0f);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    } while (keepGoing);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}