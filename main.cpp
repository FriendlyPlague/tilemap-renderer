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
const float SCALE = 3;

// typedef struct sqrHitbox {
//     float x;
//     float y;
//     float w;
//     float h;
// } sqrHitbox;

const float SPEED = .3;

int layer1[MAP_H*MAP_W];
int layer2[MAP_H*MAP_W];

bool storeCsv(int* arr,string mapPath, int cw, int ch);
int animate(double* dt);


int main( int argc, char* args[] )
{
    Renderer wind(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE,16);
    if(!wind.init() )
    {
        printf( "Failed to initialize!\n" );
        return 1;
    }
    if(!wind.loadMedia()) {
        printf( "Failed to load media!\n" );
        return 1;
    }
    if (!storeCsv(layer1,"Assets/background_Tile Layer 1.csv", MAP_W, MAP_H)) {
        printf("layer 1 failed to load!\n");
        return 1;
    }
    if (!storeCsv(layer2,"Assets/background_Tile Layer 2.csv", MAP_W, MAP_H)) {
        printf("layer 2 failed to load!\n");
        return 1;
    }

    SDL_Rect pd; //player dst rectangle
    pd.x = (wind.cam.w)/2;
    pd.y = (wind.cam.h)/2;
    pd.h = 32*SCALE;
    pd.w = 16*SCALE;
    SDL_Rect pSrc; //player src rectangle
    pSrc.h = 32;
    pSrc.w = 16;

    bool quit = false;
    SDL_Event e;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    while (!quit) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency());
        //Handle events on queue
        while( SDL_PollEvent(&e) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
                return 0;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL); //get state of keyboard
        int a_frame = animate(&deltaTime);
        int dirs = currentKeyStates[SDL_SCANCODE_DOWN] + currentKeyStates[SDL_SCANCODE_UP] + currentKeyStates[SDL_SCANCODE_RIGHT] + currentKeyStates[SDL_SCANCODE_LEFT];
        float cspeed = SPEED;
        if (dirs > 1) {
            cspeed /= 1.5;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            wind.cam.y += cspeed * deltaTime;
            pSrc.x = a_frame * 16;
            pSrc.y = 0;
        }
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            wind.cam.y -= cspeed * deltaTime;
            pSrc.x = a_frame * 16;
            pSrc.y = 64;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            wind.cam.x += cspeed * deltaTime;
            pSrc.x = a_frame * 16;
            pSrc.y = 32;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            wind.cam.x -= cspeed * deltaTime;
            pSrc.x = a_frame * 16;
            pSrc.y = 96;
        }
        pd.x = (wind.cam.w)/2;
        pd.y = (wind.cam.h)/2;
        wind.renderAll(layer1, layer2, &pSrc, &pd); // Renders all layers
    }
}

bool storeCsv(int* arr,string mapPath, int cw, int ch) {
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

int animate(double* dt) {
    static double a = 0;
    a += (1/90.0f) * (*dt);
    if (!(a < 4.0f)) {
        a = 0.0f;
    }
    return (int)a;
}