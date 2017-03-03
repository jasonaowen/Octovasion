#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

int main(int argc, const char * argv[]) {
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
        return 1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int winWidth = 770;
    int winHeight = 640;

    window = SDL_CreateWindow("GIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool gameIsRunning = true;
    while(gameIsRunning)
    {
        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
            if ((event.type == SDL_QUIT)||(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
            {
                gameIsRunning = false;
                break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_Rect rect = {0, 0, winWidth, winHeight};
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
