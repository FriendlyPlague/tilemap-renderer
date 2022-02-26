#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Renderer {
    public:
        //Key press surfaces constants
        enum KeyPressSurfaces
        {
            KEY_PRESS_SURFACE_DEFAULT,
            KEY_PRESS_SURFACE_UP,
            KEY_PRESS_SURFACE_DOWN,
            KEY_PRESS_SURFACE_LEFT,
            KEY_PRESS_SURFACE_RIGHT,
            KEY_PRESS_SURFACE_TOTAL
        };
    private:
        int screen_width;
        int screen_height;
        SDL_Window* gWindow = NULL;
        SDL_Surface* gScreenSurface = NULL;
        SDL_Surface* gCurrentSurface = NULL;
        SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

    public:
        Renderer(int width, int height);
        ~Renderer();

        bool init();
        bool loadMedia();
        SDL_Surface* loadSurface(string path);
        bool applySurface();
        void setCurrentSurface(int s_num);
        void close();
};

#endif // RENDERER_H