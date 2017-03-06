#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
        return 1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int winWidth = 770;
    int winHeight = 640;

    window = SDL_CreateWindow("GIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    const std::string filename = "../../purplefriend3x.png";
    SDL_Texture *octofriend = IMG_LoadTexture(renderer, filename.c_str());
    if ( octofriend == nullptr )
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            fprintf(stdout, "Current working dir: %s\n", cwd);

        std::cerr << "Failed to load texture " << filename << " error : " << IMG_GetError() << std::endl;
    }

    int w; int h;
    SDL_QueryTexture(octofriend, NULL, NULL, &w, &h);
    SDL_Rect rect = {
        (winWidth - w)/2,
        winHeight - 2*h/3,
        w,
        h
    };

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
        SDL_RenderCopy(renderer, octofriend, NULL, &rect);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
