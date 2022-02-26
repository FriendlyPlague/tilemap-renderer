#include "renderer.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>


Renderer::Renderer(int width, int height) 
{
    screen_width = width;
    screen_height = height;
}

Renderer::~Renderer() 
{
    close();
}

bool Renderer::init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }
    return success;
}

bool Renderer::loadMedia() 
{
    //Loading success flag
    bool success = true;
    gPlayerSurfaces[PLAYER_DEFAULT] = loadSurface("Assets/default.png");
    gPlayerSurfaces[PLAYER_DOWN] = loadSurface("Assets/down.png");
    gPlayerSurfaces[PLAYER_UP] = loadSurface("Assets/up.png");
    gPlayerSurfaces[PLAYER_LEFT] = loadSurface("Assets/left.png");
    gPlayerSurfaces[PLAYER_RIGHT] = loadSurface("Assets/right.png");
    for (int i = 0; i < PLAYER_TOTAL; i++) {
        if (gPlayerSurfaces[i] == NULL) {
            success = false;
        }
    }
    return success;
}

bool Renderer::applySurface(SDL_Rect* posRect) 
{
    bool success = true;
    posRect->w = gCurrentSurface->w;
    posRect->h = gCurrentSurface->h;
    SDL_FillRect(gScreenSurface, NULL, 0x000000);
    SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, posRect);
    SDL_UpdateWindowSurface(gWindow);
    return success;
}

void Renderer::close()
{
    //Deallocate surfaces
    for (int i = 0; i < PLAYER_TOTAL; i++) {
        SDL_FreeSurface(gPlayerSurfaces[i]);
        gPlayerSurfaces[i] = NULL;
    }
    gCurrentSurface = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void Renderer::setCurrentSurface(int s_num)
{
    gCurrentSurface = gPlayerSurfaces[s_num];
}

SDL_Surface* Renderer::loadSurface(string path)
{
    //final optimized surface
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Could not load: %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        //get rid of old surface
        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;
    }
    return optimizedSurface;
}
