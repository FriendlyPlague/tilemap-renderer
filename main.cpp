//Using SDL and standard IO
#include "renderer.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <fstream>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Map size constants
const int MAP_W = 100;
const int MAP_H = 100;

typedef struct sqrHitbox{
    float x;
    float y;
    float w;
    float h;
} sqrHitbox;

const float SPEED = 2;

int* layer1 = (int*)malloc(sizeof(int)*MAP_H*MAP_W);
int* layer2 = (int*)malloc(sizeof(int)*MAP_H*MAP_W);

bool store_csv(int* arr,string mapPath, int cw, int ch);

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
    if (!store_csv(layer1,"Assets/background_Tile Layer 1.csv", MAP_W, MAP_H)) {
        printf("layer 1 failed to load!\n");
        return 1;
    }
    if (!store_csv(layer2,"Assets/background_Tile Layer 2.csv", MAP_W, MAP_H)) {
        printf("layer 2 failed to load!\n");
        return 1;
    }
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        Uint64 start = SDL_GetPerformanceCounter(); //start timer
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
                free(layer1);
                free(layer2);
                return 0;
            }
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL); //get state of keyboard
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            wind.cam.y += SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            wind.cam.y -= SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            wind.cam.x += SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            wind.cam.x -= SPEED;
        }

        wind.renderAll(layer1, layer2); // Renders all layers

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
        printf("current framerate: %f\n", 1.0f/ elapsed);
        // SDL_Delay(floor(16.666f - elapsedMS));
        // end = SDL_GetPerformanceCounter();
        // elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency();
        // printf("Current FPS: %f\n",1.0f/elapsedMS);
    }
    free(layer1);
    free(layer2);
}

bool store_csv(int* arr,string mapPath, int cw, int ch) {
    ifstream csvFile(mapPath);
    for (int lh = 0; lh < ch; lh++) {
        for (int lw = 0; lw < cw; lw++) {
            string tmp;
            if (lw == cw-1) {
                if(!getline(csvFile,tmp)) {
                    printf("Invalid map dimensions!\n");
                    csvFile.close();
                    return false;
                }
            }
            else if (!getline(csvFile,tmp, ',')) {
                printf("Invalid map dimensions!\n");
                csvFile.close();
                return false;
            }
            int loc = stoi(tmp);
            arr[cw*lh+lw] = loc;
        }
    }
    csvFile.close();
    return true;
}