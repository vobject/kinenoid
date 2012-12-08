#include "SdlWindow.h"

namespace kinectlib {

SdlWindow::SdlWindow( const int xres, const int yres )
   : mXRes(xres)
   , mYRes(yres)
   , mScreen(NULL)
   , mSurface(NULL)
   , mFont(NULL)
{
   // Bring up SDL's video system...
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit); // ...make sure everything gets cleaned up at exit...

   // ...and finally create the window.
   mScreen = SDL_SetVideoMode(mXRes, mYRes, 0, 0);
   if (!mScreen) {
      throw "SDL_SetVideoMode() failed.";
   }

   // This is the surface that we will write to.
   mSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   mXRes,
                                   mYRes,
                                   24,
                                   0x000000ff,
                                   0x0000ff00,
                                   0x00ff0000,
                                   0x00000000);
   if (!mSurface) {
      throw "SDL_CreateRGBSurface() failed.";
   }

   if (0 > TTF_Init()) {
      throw "Cannot init SDL ttf feature.";
   }

   mFont = TTF_OpenFont("VeraMono.ttf", 16);

   if (!mFont) {
      TTF_Quit();
      throw "TTF_OpenFont() failed!";
   }
}

SdlWindow::~SdlWindow()
{
   SDL_FreeSurface(mScreen);
   SDL_FreeSurface(mSurface);

   TTF_CloseFont(mFont);
   TTF_Quit();
}

void* SdlWindow::GetSurfacePtr() const
{
   return mSurface->pixels;
}

void SdlWindow::Flip() const
{
   SDL_BlitSurface(mSurface, NULL, mScreen, NULL);
   SDL_Flip(mScreen);
}

void SdlWindow::DrawRect(
   const Sint16 x_pos,
   const Sint16 y_pos,
   const Uint16 size,
   const Uint32 color /* = 0xffffffff */
) const
{
   SDL_Rect rect = { x_pos - (size / 2), y_pos - (size / 2), size, size };
   SDL_FillRect(mSurface, &rect, color);
}

void SdlWindow::WriteText(
   const Sint16 x_pos,
   const Sint16 y_pos,
   const SDL_Color color,
   const std::string& text
) const
{
   SDL_Surface* surface = TTF_RenderText_Blended(mFont, text.c_str(), color);
   SDL_Rect rect = { x_pos, y_pos, 0, 0 };

   SDL_BlitSurface(surface, NULL, mSurface, &rect);
   SDL_FreeSurface(surface);
}

} // namespace op
