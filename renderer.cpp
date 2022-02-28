#include "renderer.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <fstream>
#include <ios>


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

bool Renderer::renderMap(SDL_Texture* srcMap,string mapPath, int cw, int ch)
{
    /*renders every thing in csv file with corresponding texture left to right,
    top to bottom*/
    ifstream csvFile(mapPath);
    if (!csvFile.is_open()) {
        printf("Could not open: %s\n",mapPath.c_str());
        return false;
    }
    int tw;
    SDL_QueryTexture(srcMap,NULL, NULL, &tw, NULL);
    tw /= tileSize;
    bool success = true;
    for (int lh = 0; lh < ch; lh++) {
        for (int lw = 0; lw < cw; lw++) {
            string tmp;
            if (lw == cw-1) {
                if(!getline(csvFile,tmp)) {
                    printf("Invalid map dimensions!\n");
                    return false;
                }
            }
            else if (!getline(csvFile,tmp, ',')) {
                printf("Invalid map dimensions!\n");
                return false;
            }
            int loc = stoi(tmp);
            if (loc == -1) {
                continue;
            }
            SDL_Rect dRect;
            dRect.x = (lw * tileSize * scale) - cam.x;
            dRect.y = (lh * tileSize * scale) - cam.y;
            dRect.w = tileSize*scale, dRect.h = tileSize*scale;
            if (dRect.x < -(tileSize * scale) || dRect.y < -(tileSize * scale) || dRect.x > cam.w) {
                continue;
            }
            else if (dRect.y > cam.h) {
                return true;
            }
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
    csvFile.close();
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

bool Renderer::renderAll()
{
    SDL_RenderClear(gRenderer);
    renderMap(tMap, "Assets/background_Tile Layer 1.csv", 100, 100);
    renderMap(tMap, "Assets/background_Tile Layer 2.csv", 100, 100);
    SDL_RenderPresent(gRenderer);
    return true;
}