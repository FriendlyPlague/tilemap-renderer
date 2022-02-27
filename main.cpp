//Using SDL and standard IO
#include "renderer.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

typedef struct player{
    int x = 0;
    int y = 0;
}player;

int main( int argc, char* args[] )
{
    Renderer wind(SCREEN_WIDTH, SCREEN_HEIGHT, 2,16);
    if(!wind.init() )
    {
        printf( "Failed to initialize!\n" );
        return 1;
    }
    if(!wind.loadMedia()) {
        printf( "Failed to load media!\n" );
        return 1;
    }
    wind.loadMedia();
    wind.renderAll();
    SDL_Delay(5000);
}
