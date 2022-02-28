#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Renderer {
    private:
        typedef struct camera {
            int x = 0;
            int y = 0;
            int w;
            int h;
        } camera;
    public: camera cam;
    private:
        int screen_width;
        int screen_height;
        int tileSize;
        int scale;
        SDL_Window* gWindow = NULL;
        SDL_Surface* gScreenSurface = NULL;
        SDL_Texture* gCurrentSurface = NULL;
        SDL_Renderer* gRenderer = NULL;
        SDL_Texture* gTexture = NULL;
        SDL_Texture* tMap;

    public:
        Renderer(int width, int height, float scale, int tilew);
        ~Renderer();

        bool init();
        bool loadMedia();
        bool applySurface(SDL_Rect* posRect);
        bool renderMap(SDL_Texture* srcMap, int* mapArr, int cw, int ch);
        SDL_Texture* loadTexture(string path);
        bool renderAll(int* layer1, int* layer2);
        void setCurrentTexture(int s_num);
        void close();
};

#endif // RENDERER_H