#include "DebugOverlayDrawer.h"
#include "SdlWindow.h"

namespace kinectlib {

DebugOverlayDrawer::DebugOverlayDrawer(KinectWrapper* subject, const SdlWindow& wnd)
   : mSubject(subject)
   , mWindow(wnd)
   , mOverlayMode(dom_JointsRealWorld)
{
   mSubject->Subscribe(this);

   mTextColor.r = 0xff;
   mTextColor.g = 0xff;
   mTextColor.b = 0xff;
}

DebugOverlayDrawer::~DebugOverlayDrawer()
{
   if (mSubject) {
      mSubject->Unsubscribe(this);
   }
}

void DebugOverlayDrawer::Update( const Subject* updated_subject )
{
   if (updated_subject != mSubject) {
      // This is not the subject that we registered to.
      throw "Invalid subject.";
   }

   if (dom_None == mOverlayMode) {
      return;
   }

   DrawOverlay();
}

DebugOverlayMode DebugOverlayDrawer::SwitchOverlayMode()
{
   mOverlayMode = (DebugOverlayMode)((mOverlayMode + 1) % DEBUG_OVERLAY_MODE_ITEM_COUNT);
   return mOverlayMode;
}

void DebugOverlayDrawer::DrawOverlay()
{
   std::vector<kinectlib::UserData> users;
   mSubject->GetUsers(users);

   for (std::vector<UserData>::const_iterator user_iter = users.begin();
        user_iter != users.end();
        ++user_iter)
   {
      std::map<int, XnPoint3D> joints = user_iter->GetRealWorldJoints();

      // TODO: This is ugly and slow. Think of a better way to do this.
      for (int i = XN_SKEL_HEAD; i <= XN_SKEL_RIGHT_FOOT; ++i)
      {
         mTextBuf.str("");
         const std::map<int, XnPoint3D>::const_iterator it = joints.find(i);
         const std::string joint_name = user_iter->JointId2JointName(i);

         if (it == joints.end())
         {
            mTextBuf << joint_name << " (-1, -1, -1)";
         }
         else
         {
            XnPoint3D joint = it->second;
            if (dom_JointsProjective == mOverlayMode)
            {
               joint = mSubject->RealWorldToProjective(joint);
            }

            mTextBuf << joint_name << " ("
                                   << (int)joint.X << ", "
                                   << (int)joint.Y << ", "
                                   << (int)joint.Z << ")";
         }

         mWindow.WriteText(0, i * 18 + 10, mTextColor, mTextBuf.str());
      }
   }
}

} // namespace kinectlib
