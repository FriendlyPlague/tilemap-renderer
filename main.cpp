//Using SDL and standard IO
#include "renderer.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
                    wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_UP);
                    break;

                    case SDLK_DOWN:
                    wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_DOWN);
                    break;

                    case SDLK_LEFT:
                    wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_LEFT);
                    break;

                    case SDLK_RIGHT:
                    wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_RIGHT);
                    break;

                    default:
                    wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_DEFAULT);
                    break;
                }
            }
            else {
                wind.setCurrentSurface(wind.KEY_PRESS_SURFACE_DEFAULT);
            }
        }
        wind.applySurface();
    }
}
