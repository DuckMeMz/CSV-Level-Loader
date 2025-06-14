#include "Sprite.h"

Sprite::Sprite()
{
    m_pSurface = nullptr;
}

Result Sprite::Load(const char* filename, Uint32 pixelFormat)
{
    SDL_Surface* pTempSurface = nullptr;        // Use this to load the bitmap
    // Load the file
    pTempSurface = SDL_LoadBMP(filename);
    if (!pTempSurface)
    {
        // Surface could no load from file. No file, or not an image, or something
        // Report the error
        return Result::FAILURE;       // Return failure
    }

    // Now need to convert the format of the loaded surface to match the display format 
    // (such as colour depth, etc)
    // Not required, but will make the rendering faster if we do this
    // pixelformat is just a number that tells the system which format is being used
    // 0 is for flags, but these are not used and are just there for backwards compatibility
    m_pSurface = SDL_ConvertSurfaceFormat(pTempSurface, pixelFormat, 0);

    // Need to free up the resource used by the temporary surface
    SDL_FreeSurface(pTempSurface);

    return Result::FAILURE;
}

Result Sprite::Draw(SDL_Surface* pTargetSurface, int x, int y)
{
    if (pTargetSurface == NULL || m_pSurface == NULL) 
    {
        // Either no picture or not window to draw to
        // Report error
        // Return with error
        return Result::FAILURE;
    }

    // Create a destination rectangle
    // Presumable can scale at this point, but whatever
    SDL_Rect DestR;

    DestR.x = x;
    DestR.y = y;

    // Try to blit. No error codes?
    SDL_BlitSurface(m_pSurface, NULL, pTargetSurface, &DestR);

    return Result::SUCCESS;
}
