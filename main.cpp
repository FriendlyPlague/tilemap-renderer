//Using SDL and standard IO
#include "renderer.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

typedef struct {
    int x = 0;
    int y = 0;
}player;

int main( int argc, char* args[] )
{
    Renderer wind(SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!wind.init() )
    {
        printf( "Failed to initialize!\n" );
        return 1;
    }
    if(!wind.loadMedia()) {
        printf( "Failed to load media!\n" );
        return 1;
    }
    //Apply the image
    player p;
    SDL_Rect rect;
    bool quit = false;
    while(!quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            //user requests to quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym)
                {
                    case SDLK_UP:
                    wind.setCurrentSurface(wind.PLAYER_UP);
                    p.y -= 10;
                    break;

                    case SDLK_DOWN:
                    wind.setCurrentSurface(wind.PLAYER_DOWN);
                    p.y += 10;
                    break;

                    case SDLK_LEFT:
                    wind.setCurrentSurface(wind.PLAYER_LEFT);
                    p.x -= 10;
                    break;

                    case SDLK_RIGHT:
                    wind.setCurrentSurface(wind.PLAYER_RIGHT);
                    p.x += 10;
                    break;

                    default:
                    wind.setCurrentSurface(wind.PLAYER_DEFAULT);
                    break;
                }
            }
            else {
                wind.setCurrentSurface(wind.PLAYER_DEFAULT);
            }
        }
        rect.x = p.x;
        rect.y = p.y;
        wind.applySurface(&rect);
    }
}
