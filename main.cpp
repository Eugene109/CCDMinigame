#include <iostream>
#include <stdio.h>
#include <thread>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"

using namespace std;

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool up;
    bool down;
    bool left;
    bool right;
} App;

App app;

#include "player.h"

#define SCR_W 1280
#define SCR_H 720

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 50, 255, 50, 255);
    SDL_RenderClear(app.renderer);
}

void initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "Unable to initialize SDL: " << SDL_GetError();
        exit(1);
    }
    app.window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_W, SCR_H, 0);
    if (!app.window)
    {
        cout << "Failed to create window\n";
        exit(1);
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void cleanup()
{
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    memset(&app, 0, sizeof(App));

    initSDL();

    atexit(cleanup);

    SDL_Surface *downloadRAM = SDL_LoadBMP("downloadRAM.bmp");
    SDL_Texture *downloadRAM_tex = SDL_CreateTextureFromSurface(app.renderer, downloadRAM);

    SDL_Surface *frame0 = SDL_LoadBMP("frame0.bmp");
    SDL_Texture *frame0_tex = SDL_CreateTextureFromSurface(app.renderer, frame0);
    SDL_Surface *frame1 = SDL_LoadBMP("frame1.bmp");
    SDL_Texture *frame1_tex = SDL_CreateTextureFromSurface(app.renderer, frame1);
    SDL_Surface *frame2 = SDL_LoadBMP("frame2.bmp");
    SDL_Texture *frame2_tex = SDL_CreateTextureFromSurface(app.renderer, frame2);
    SDL_Surface *frame3 = SDL_LoadBMP("frame3.bmp");
    SDL_Texture *frame3_tex = SDL_CreateTextureFromSurface(app.renderer, frame3);

    SDL_Texture *nyan_cat_anim[] = {frame0_tex, frame1_tex, frame2_tex, frame3_tex};
    unsigned char frame = 0;

    Player player(640, 360, "Pheonix_Holmes_Stationary.png", "running.gif");

    unsigned int a = SDL_GetTicks();
    unsigned int b = SDL_GetTicks();
    double delta = 0;

    bool running = true;
    while (running)
    {
        a = SDL_GetTicks();
        delta = a - b;

        if (delta > 1000 / 60.0)
        {
            b = a;

            /*
            HANDLE EVENTS

            */
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode)
                    {
                    // case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_W:
                        app.up = true;
                        break;
                    // case SDL_SCANCODE_DOWN:
                    case SDL_SCANCODE_S:
                        app.down = true;
                        break;
                    // case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_A:
                        app.left = true;
                        break;
                    // case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                        app.right = true;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.scancode)
                    {
                    // case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_W:
                        app.up = false;
                        break;
                    // case SDL_SCANCODE_DOWN:
                    case SDL_SCANCODE_S:
                        app.down = false;
                        break;
                    // case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_A:
                        app.left = false;
                        break;
                    // case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                        app.right = false;
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }

            prepareScene();

            /*
            DRAW THINGS

            */
            player.draw();
            player.update(delta);

            SDL_RenderPresent(app.renderer);
            SDL_UpdateWindowSurface(app.window);
        }
    }

    SDL_DestroyTexture(downloadRAM_tex);
    SDL_DestroyTexture(frame0_tex);
    SDL_DestroyTexture(frame1_tex);
    SDL_DestroyTexture(frame2_tex);
    SDL_DestroyTexture(frame3_tex);
    SDL_FreeSurface(downloadRAM);
    SDL_FreeSurface(frame0);
    SDL_FreeSurface(frame1);
    SDL_FreeSurface(frame2);
    SDL_FreeSurface(frame3);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}