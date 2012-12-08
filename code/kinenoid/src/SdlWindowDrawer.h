#ifndef __SDL_WINDOW_DRAWER_H__
#define __SDL_WINDOW_DRAWER_H__

#include "Observer.h"
#include "KinectWrapper.h"

namespace kinectlib {

class SdlWindow;

enum DisplayMode
{
   dm_None,
   dm_Image,
   dm_Depth,
   DISPLAY_MODE_ITEM_COUNT
};

class SdlWindowDrawer : public Observer
{
public:
   SdlWindowDrawer(KinectWrapper* subject, const SdlWindow& wnd);
   virtual ~SdlWindowDrawer();

   virtual void Update(const Subject* updated_subject);

   DisplayMode SwitchDisplayMode();
   int IncreaseDepth( int plus = 200 );
   int DecreaseDepth( int minus = 200 );

private:
   void DrawImage();
   void DrawDepthMap();

   KinectWrapper* const mSubject;
   const SdlWindow& mWindow;
   const int mXRes;
   const int mYRes;

   DisplayMode mDisplayMode;
   size_t mMaxDepth; // Distance in millimeters for depth mode.

   SdlWindowDrawer& operator=(const SdlWindowDrawer&);
};

} // namespace kinectlib

#endif // __SDL_WINDOW_DRAWER_H__
