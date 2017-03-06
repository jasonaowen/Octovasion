#include <iostream>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "GameState.hpp"

#ifdef __APPLE__
#  include <SDL2_Image/SDL_image.h>
#  define ASSET_DIR "../.."
#else
#  include <SDL2/SDL_image.h>
#  define ASSET_DIR ".."
#endif

SDL_Rect gameRectToScreenRect(SDL_Rect screen, Rect world, Rect rect) {
  float xScale = (float)(screen.w - screen.x) / (world.width - world.x);
  float yScale = (float)(screen.h - screen.y) / (world.height - world.y);
  return {
    (int)(rect.x * xScale + screen.x),
    (int)(rect.y * yScale + screen.y),
    (int)(rect.width * xScale),
    (int)(rect.height * yScale)
  };
}

void render(SDL_Renderer* renderer, GameState state, SDL_Texture* octofriend) {
  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);
  SDL_Rect screen = {
    0,
    0,
    width,
    height
  };
  Rect world = {
    0,
    0,
    state.worldWidth,
    state.worldHeight
  };

  int imgWidth; int imgHeight;
  SDL_QueryTexture(octofriend, NULL, NULL, &imgWidth, &imgHeight);
  SDL_Rect rect = {
      (width - imgWidth)/2,
      height - 2*imgHeight/3,
      imgWidth,
      imgHeight
  };
  SDL_RenderCopy(renderer, octofriend, NULL, &rect);

  // render paddle
  SDL_SetRenderDrawColor(renderer, 0xA1, 0x57, 0xE8, 0xFF);
  SDL_Rect paddleRect = gameRectToScreenRect(screen, world, state.paddle);
  SDL_RenderFillRect(renderer, &paddleRect);

  // render the bullets
  for (Point bullet : state.bullets) {
    SDL_Rect bulletRect = gameRectToScreenRect(screen, world, {
      bullet.x,
      bullet.y,
      1,
      1
    });
    SDL_RenderCopy(renderer, octofriend, NULL, &bulletRect);
  }

  // render babbies
}

int main(int argc, const char * argv[]) {
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
        return 1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int winWidth = 770;
    int winHeight = 640;

    window = SDL_CreateWindow("GIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    const std::string filename = ASSET_DIR "/purplefriend3x.png";
    SDL_Texture *octofriend = IMG_LoadTexture(renderer, filename.c_str());
    if ( octofriend == nullptr )
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            fprintf(stdout, "Current working dir: %s\n", cwd);

        std::cerr << "Failed to load texture " << filename << " error : " << IMG_GetError() << std::endl;
    }

    GameState state = GameState(20, 20);
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);
        render(renderer, state, octofriend);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
