/*
 *  Octovasion - an RC Game Jam game!
 *  Copyright (C) 2017 Gwendolyn Weston, Jason Owen, Xianny Ng
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "GameState.hpp"
#include <map>
#include <string>
#include <cstdlib>

#ifdef __APPLE__
#  include <SDL2_Image/SDL_image.h>
#  define ASSET_DIR "../../assets"
#else
#  include <SDL2/SDL_image.h>
#  define ASSET_DIR "../assets"
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

void render(SDL_Renderer* renderer, GameState state, std::map<std::string, SDL_Texture *> assets) {
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
  SDL_QueryTexture(assets["purple"], NULL, NULL, &imgWidth, &imgHeight);
  SDL_Rect rect = {
      (width - imgWidth)/2,
      height - 2*imgHeight/3,
      imgWidth,
      imgHeight
  };
  SDL_RenderCopy(renderer, assets["purple"], NULL, &rect);

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
    SDL_RenderCopy(renderer, assets["sunshine"], NULL, &bulletRect);
  }

  // render babbies
  for (int i = 0; i < state.capturedOctobabies; i++) {
    SDL_Rect babyRect = gameRectToScreenRect(screen, world, {
      i % world.width,
      i / world.width,
      1,
      1
    });
    SDL_RenderCopy(renderer, assets["pink"], NULL, &babyRect);
  }
}

int main(int argc, const char * argv[]) {
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
        return 1;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int winWidth = 770;
    int winHeight = 640;

    window = SDL_CreateWindow("Octovasion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    const std::string filename = ASSET_DIR "/purplefriend3x.png";
    std::map<std::string, SDL_Texture *> assets = {
        {"purple", IMG_LoadTexture(renderer, ASSET_DIR "/purplefriend3x.png")},
        {"pink", IMG_LoadTexture(renderer, ASSET_DIR "/pinkfriend.png")},
        {"blue", IMG_LoadTexture(renderer, ASSET_DIR "/bluefriend.png")},
        {"lime", IMG_LoadTexture(renderer, ASSET_DIR "/limefriend.png")},
        {"sunshine", IMG_LoadTexture(renderer, ASSET_DIR "/sunshinefriend.png")},
        {"skyblue", IMG_LoadTexture(renderer, ASSET_DIR "/skybluefriend.png")},
    };

    for (auto const &asset: assets) {
        if ( asset.second == nullptr )
        {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                fprintf(stdout, "Current working dir: %s\n", cwd);

            std::cerr << "Failed to load texture " << asset.first << " error : " << IMG_GetError() << std::endl;
        }
    }

    GameState state = GameState(20, 20);
    bool gameIsRunning = true;

    const Uint32 fps = 40;
    const Uint32 minframetime = 1000/fps;

    Uint32 frames = 0;
    int chance = 2;
    const Uint8 *keyboardState;

    Uint32 prevtick = 0;


    Action platformHeldAction;
    Action bulletsHeldAction;

    while(gameIsRunning)
    {
        Uint32 frametime = SDL_GetTicks();
        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
            if ((event.type == SDL_QUIT)||(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
            {
                gameIsRunning = false;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_n) {
                    state.handleAction(Action::NEW_GAME);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    platformHeldAction = Action::LEFT;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    platformHeldAction = Action::RIGHT;
                }
                if (event.key.keysym.sym == SDLK_j) {
                    bulletsHeldAction = Action::FIRE_LEFT_BULLET;
                }
                if (event.key.keysym.sym == SDLK_l) {
                    bulletsHeldAction = Action::FIRE_RIGHT_BULLET;
                }
            }
        }

        keyboardState = SDL_GetKeyboardState(NULL);

        if (keyboardState[SDL_SCANCODE_D] && keyboardState[SDL_SCANCODE_A]) {
            state.handleAction(platformHeldAction);
        } else if (keyboardState[SDL_SCANCODE_D]) {
            state.handleAction(Action::RIGHT);
        } else if (keyboardState[SDL_SCANCODE_A]) {
            state.handleAction(Action::LEFT);
        } else {
            state.handleAction(Action::CENTER);
        }

        if (keyboardState[SDL_SCANCODE_L] && keyboardState[SDL_SCANCODE_J]) {
            prevtick = SDL_GetTicks();
            state.handleAction(bulletsHeldAction);
        } else if (keyboardState[SDL_SCANCODE_L]) {
            prevtick = SDL_GetTicks();
            state.handleAction(Action::FIRE_RIGHT_BULLET);
        } else if (keyboardState[SDL_SCANCODE_J]) {
            prevtick = SDL_GetTicks();
            state.handleAction(Action::FIRE_LEFT_BULLET);
        }

        if (SDL_GetTicks() > prevtick + 5000)
        {
            if (frames % 10 == 0)
            {
                if (rand() % chance == 0) {
                    state.handleAction(Action::FIRE_LEFT_BULLET);
                }
                if (rand() % chance == 0) {
                    state.handleAction(Action::FIRE_RIGHT_BULLET);
                }
            }
        }

        if (SDL_GetTicks() - frametime < minframetime)
        {
            SDL_Delay(minframetime - (SDL_GetTicks() - frametime));
        }

        if (frames % 5 == 0) {
            state.handleAction(Action::MOVE_BULLET);
            state.handleAction(Action::CHECK_PADDLE_COLLISION);
            state.handleAction(Action::ESCAPE_BULLET);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);
        render(renderer, state, assets);
        frames++;

        SDL_RenderPresent(renderer);
    }

    return 0;
}
