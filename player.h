#ifndef PLAYER_H
#define PLAYER_H

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include <string>

using namespace std;

class Entity
{
public:
    int x;
    int y;
    SDL_Texture *texture;
    Entity()
    {
        x = 0;
        y = 0;
    }
    Entity(int x_in, int y_in, string filename) : x(x_in), y(y_in)
    {
        texture = IMG_LoadTexture(app.renderer, filename.c_str());
        // IMG_Animation *anim = IMG_LoadAnimation("pheonixHolmes.png");
        // anim->frames
    }
    ~Entity()
    {
        SDL_DestroyTexture(texture);
    }

    void draw()
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(app.renderer, texture, NULL, &dest);
    }
};

class Player : public Entity
{
public:
    IMG_Animation *anim;
    int frame;
    double currentDelta;
    SDL_Texture *ogTex;
    SDL_Texture *hideTex;
    Player(int x_in, int y_in, string filename, string gif) : Entity(x_in, y_in, filename)
    {
        ogTex = texture;
        hideTex = IMG_LoadTexture(app.renderer, "hide.png");
        // setup anim
        anim = IMG_LoadAnimation(gif.c_str());
        frame = 0;
        currentDelta = 0;
    }
    void update(double delta_t)
    {
        y -= app.up * 1;
        y += app.down * 1;
        x -= app.left * 1;
        x += app.right * 1;
        if ((app.up xor app.down) || (app.left xor app.right))
        {
            currentDelta += delta_t / 1000.0;
            if (currentDelta >= 0.35)
            {
                currentDelta = 0;
                frame++;
                if (texture != ogTex)
                    SDL_DestroyTexture(texture);
                texture = SDL_CreateTextureFromSurface(app.renderer, anim->frames[frame % anim->count]);
            }
        }
        else
        {
            frame = 0;
            currentDelta = 0.35;
        }
    }
    void draw()
    {
        SDL_Rect dest;

        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        dest.w /= 4;
        dest.h /= 4;
        dest.x = x - dest.w / 2;
        dest.y = y - dest.h / 2;

        double angle = 0;
        if (app.up && !app.down)
        {
            angle = 20;
        }
        if (app.down && !app.up)
        {
            angle = -20;
        }
        if (app.left)
        {
            SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_HORIZONTAL);
        }
        else
        {

            SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
        }

        dest.x = x - 1280 * 2;
        dest.y = y - 720 * 2;
        dest.w = 2560 * 2;
        dest.h = 1440 * 2;
        SDL_RenderCopy(app.renderer, hideTex, NULL, &dest);
    }
};

#endif