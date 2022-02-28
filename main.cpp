//Using SDL and standard IO
#include "renderer.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

typedef struct sqrHitbox{
    float x;
    float y;
    float w;
    float h;
} sqrHitbox;

int main( int argc, char* args[] )
{
    Renderer wind(SCREEN_WIDTH, SCREEN_HEIGHT, 2.5,16);
    if(!wind.init() )
    {
        printf( "Failed to initialize!\n" );
        return 1;
    }
    if(!wind.loadMedia()) {
        printf( "Failed to load media!\n" );
        return 1;
    }
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
                return 0;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                    wind.cam.y -= 10;
                    break;
                    case SDLK_DOWN:
                    wind.cam.y += 10;
                    break;
                    case SDLK_RIGHT:
                    wind.cam.x += 10;
                    break;
                    case SDLK_LEFT:
                    wind.cam.x -= 10;
                    break;
                }
            }
        }
        wind.renderAll();
    }
}
