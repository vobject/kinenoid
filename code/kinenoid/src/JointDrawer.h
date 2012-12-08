#ifndef __JOINT_DRAWER_H__
#define __JOINT_DRAWER_H__

#include "Observer.h"
#include "KinectWrapper.h"

namespace kinectlib {

class SdlWindow;

enum UserPaintMode {
   upm_None,
   upm_Dots,
   USER_PAINT_MODE_ITEM_COUNT
};

class JointDrawer : public Observer
{
public:
   JointDrawer(KinectWrapper* subject, const SdlWindow& wnd);
   virtual ~JointDrawer();

   virtual void Update(const Subject* updated_subject);

   UserPaintMode SwitchUserPaintMode();

private:
   void DrawJoints() const;

   KinectWrapper* const mSubject;
   const SdlWindow& mWindow;
   const int mXRes;
   const int mYRes;

   UserPaintMode mUserPaintMode;

   JointDrawer& operator=(const JointDrawer&);
};

} // namespace kinectlib

#endif // __JOINT_DRAWER_H__
