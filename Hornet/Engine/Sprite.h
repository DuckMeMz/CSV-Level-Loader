#pragma once
#include "SDL.h"
#include "Result.h"

class Sprite
{
public:
    Sprite();
    Result Load(const char* filename, Uint32 pixelFormat);
    Result Draw(SDL_Surface* SurfDest, int x, int y);
private:
    SDL_Surface* m_pSurface;
};
