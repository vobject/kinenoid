#include "FpsDrawer.h"
#include "SdlWindow.h"

namespace kinectlib {

FpsDrawer::FpsDrawer(KinectWrapper* subject, const SdlWindow& wnd)
   : mSubject(subject)
   , mWindow(wnd)
   , mFrameTimer(NULL)
   , mFrameCount(0)
   , mFPS(0)
{
   mSubject->Subscribe(this);

   // Bring up SDL's video system...
   if (0 > SDL_Init(SDL_INIT_TIMER)) {
      throw "Cannot init SDL timer subsystem.";
   }
   atexit(SDL_Quit); // ...make sure everything gets cleaned up at exit.

   mTextColor.r = 0xff;
   mTextColor.g = 0xff;
   mTextColor.b = 0xff;

   mFrameTimer = SDL_AddTimer(1000, FrameTimerCallback, this);
}

FpsDrawer::~FpsDrawer()
{
   SDL_RemoveTimer(mFrameTimer);

   if (mSubject) {
      mSubject->Unsubscribe(this);
   }
}

void FpsDrawer::Update( const Subject* updated_subject )
{
   if (updated_subject != mSubject) {
      // This is not the subject that we registered to.
      throw "Invalid subject.";
   }

   mTextBuf.str("");
   mTextBuf << mFPS << " FPS";
   mWindow.WriteText(0, 0, mTextColor, mTextBuf.str());

   ++mFrameCount;
}

Uint32 FpsDrawer::FrameTimerCallback( const Uint32 interval, void *param )
{
   FpsDrawer* obj = static_cast<FpsDrawer*>(param);

   obj->mFPS = (int)((obj->mFrameCount / (float)interval) * 1000);

   obj->mFrameCount = 0;
   return interval;
}

} // namespace kinectlib
