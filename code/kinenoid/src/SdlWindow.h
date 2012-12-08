#ifndef __SDL_WINDOW_H__
#define __SDL_WINDOW_H__

#include <string>

#if defined(WIN32)
#include <SDL.h>
#include <SDL_ttf.h>
#elif defined (__linux__)
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif

namespace kinectlib {

class SdlWindow
{
public:
   SdlWindow(int xres, int yres);
   ~SdlWindow();

   void* GetSurfacePtr() const;
   void DrawRect(Sint16 x_pos, Sint16 y_pos, Uint16 size, const Uint32 color = 0xffffffff) const;
   void WriteText(Sint16 x_pos, Sint16 y_pos, SDL_Color color, const std::string& text) const;
   void Flip() const;

private:
   const int mXRes;
   const int mYRes;
   SDL_Surface* mScreen;
   SDL_Surface* mSurface;

   TTF_Font* mFont;

   // Stuff we do not need atm.
   SdlWindow(const SdlWindow&);
   SdlWindow& operator=(const SdlWindow&);
};

} // namespace op

#endif // __SDL_WINDOW_H__
