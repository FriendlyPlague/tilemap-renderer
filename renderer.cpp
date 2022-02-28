#include "renderer.h"
#include <SDL2/SDL_rect.h>

Renderer::Renderer(int width, int height, float gscale, int tileW) 
{
    screen_width = width;
    screen_height = height;
    scale = gscale;
    tileSize = tileW;
    cam.w = width;
    cam.h = height;
}

Renderer::~Renderer() 
{
    close();
}

bool Renderer::init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    //Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN );
    if (gWindow == NULL) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    //create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if( gRenderer == NULL ) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    //Get window surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}

bool Renderer::loadMedia() 
{
    //Loading success flag
    tMap = loadTexture("Assets/Overworld.png");
    return true;
}

bool Renderer::renderMap(SDL_Texture* srcMap, int* mapArr, int cw, int ch)
{
    /*renders every thing in csv file with corresponding texture left to right,
    top to bottom*/
    int tw;
    SDL_QueryTexture(srcMap, NULL, NULL, &tw, NULL);
    tw /= tileSize;
    bool success = true;
    int camUX = SDL_clamp(cam.x /(tileSize*scale), 0, cw);
    int camUY = SDL_clamp(cam.y /(tileSize*scale), 0, ch);
    int maxX = SDL_clamp((cam.x+cam.w)/(tileSize*scale)+1,0 , cw);
    int maxY = SDL_clamp((cam.y+cam.h)/(tileSize*scale)+1,0 , cw);
    for (int lh = camUY; lh < maxY; lh++) {
        for (int lw = camUX; lw < maxX; lw++) {
            SDL_Rect dRect;
            dRect.x = (lw * tileSize * scale) - cam.x;
            dRect.y = (lh * tileSize * scale) - cam.y;
            dRect.w = tileSize*scale, dRect.h = tileSize*scale;
            int loc = mapArr[cw*lh+lw];
            SDL_Rect sRect;
            sRect.x = loc % tw * tileSize;
            sRect.y = loc / tw * tileSize;
            sRect.w = tileSize, sRect.h = tileSize;
            if (SDL_RenderCopy(gRenderer, srcMap, &sRect, &dRect) != 0) {
                printf("could not render texture at %d, %d! SDL error: %s",sRect.x,sRect.y, SDL_GetError());
                success = false;
            }
        }
    }
    return success;
}

bool Renderer::applySurface(SDL_Rect* posRect) 
{
    return false;
}

void Renderer::close()
{
    //Deallocate surfaces

    gCurrentSurface = NULL;
    SDL_DestroyTexture(tMap);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void Renderer::setCurrentTexture(int s_num)
{
    return;
}

SDL_Texture* Renderer::loadTexture(string path)
{
    //final texture
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Could not load: %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
        return NULL;
    }

    //create texture from surface
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
        printf("Unable to convert %s to texture! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    //get rid of old surface
    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;

    return newTexture;
}

bool Renderer::renderAll(int* layer1, int* layer2)
{
    SDL_RenderClear(gRenderer);
    renderMap(tMap, layer1, 100, 100);
    // printf("rendered layer 1\n");
    renderMap(tMap, layer2, 100, 100);
    // printf("rendered layer 2\n");
    SDL_RenderPresent(gRenderer);
    // printf("updated screen\n");
    return true;
}
