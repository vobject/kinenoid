#include "JointDrawer.h"
#include "SdlWindow.h"
#include "UserData.h"

namespace kinectlib {

JointDrawer::JointDrawer(KinectWrapper* subject, const SdlWindow& wnd)
   : mSubject(subject)
   , mWindow(wnd)
   , mXRes(subject->GetXRes())
   , mYRes(subject->GetYRes())
   , mUserPaintMode(upm_Dots)
{
   mSubject->Subscribe(this);
}

JointDrawer::~JointDrawer()
{
   if (mSubject) {
      mSubject->Unsubscribe(this);
   }
}

void JointDrawer::Update( const Subject* updated_subject )
{
   if (updated_subject != mSubject) {
      // This is not the subject that we registered to.
      throw "Invalid subject.";
   }

   if (upm_Dots == mUserPaintMode)
   {
      DrawJoints();
   }
}

UserPaintMode JointDrawer::SwitchUserPaintMode()
{
   mUserPaintMode = (UserPaintMode)((mUserPaintMode + 1) % USER_PAINT_MODE_ITEM_COUNT);
   return mUserPaintMode;
}

void JointDrawer::DrawJoints() const
{
   std::vector<UserData> users;
   mSubject->GetUsers(users);

   for (std::vector<UserData>::const_iterator user_iter = users.begin();
        user_iter != users.end();
        ++user_iter)
   {
      const std::map<int, XnPoint3D> joints = user_iter->GetRealWorldJoints();

      for (std::map<int, XnPoint3D>::const_iterator joint_iter = joints.begin();
         joint_iter != joints.end();
         ++joint_iter)
      {
         const XnPoint3D joint_proj = mSubject->RealWorldToProjective(joint_iter->second);
         mWindow.DrawRect(static_cast<Sint16>(joint_proj.X),
                          static_cast<Sint16>(joint_proj.Y),
                          6);
      }
   }
}

} // namespace kinectlib
