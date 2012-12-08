#include "UserTracking.h"

#include "Log.h"


namespace kinectlib {

UserTracking::UserTracking()
{

}

UserTracking::~UserTracking()
{
   Shutdown();
}

void UserTracking::Init( xn::Context& ctx )
{
   if (XN_STATUS_OK != mUserGen.Create(ctx)) {
      throw "xn::UserGenerator could not be inizialized. Does the context support it?";
   }

   mCallbackArgs.user_gen = &mUserGen;

   // Callback methods for the xn::UserGenerator class.
   mUserGen.RegisterUserCallbacks(UserFoundCB, UserLostCB, &mCallbackArgs, mUserGenCB);

   xn::PoseDetectionCapability pose_cap = mUserGen.GetPoseDetectionCap();
   pose_cap.RegisterToPoseCallbacks(PoseStartCB, PoseEndCB, &mCallbackArgs, mPoseCB);

   xn::SkeletonCapability skel_cap = mUserGen.GetSkeletonCap();
   skel_cap.RegisterCalibrationCallbacks(CalibStartCB, CalibEndCB, &mCallbackArgs, mCalibCB);
   skel_cap.SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
}

void UserTracking::Shutdown()
{

}

size_t UserTracking::GetUsers(std::vector<UserData>& users) const
{
   XnUserID userBuf[16];
   XnUInt16 userMax = 16;

   if (userMax < mUserGen.GetNumberOfUsers()) {
      LOG(logWARNING) << mUserGen.GetNumberOfUsers() << " users detected"
                      << " but we only support " << userMax;
   }

   mUserGen.GetUsers(userBuf, userMax);
   for (XnUInt16 id = 0; userMax >= id; ++id)
   {
      if (!mUserGen.GetSkeletonCap().IsTracking(userBuf[id])) {
         // We are only interested in the users that are being tracked.
         continue;
      }

      UserData user(userBuf[id]);

      // Show skeleton points.
      XnSkeletonJointPosition pos;
	  for (int joint = XN_SKEL_HEAD; joint <= XN_SKEL_RIGHT_FOOT; ++joint)
	  {
		  mUserGen.GetSkeletonCap().GetSkeletonJointPosition(userBuf[id], (XnSkeletonJoint)joint, pos);
		  if (0.5 > pos.fConfidence) {
			  continue;
		  }
		  user.SetRealWorldJoints(joint, pos.position);
	  }

      users.push_back(user);
   }
   return users.size();
}

void XN_CALLBACK_TYPE UserFoundCB(
   xn::UserGenerator& gen,
   XnUserID id,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "UserFoundCB()";

   gen.GetPoseDetectionCap().StartPoseDetection("Psi", id);
}

void XN_CALLBACK_TYPE UserLostCB(
   xn::UserGenerator& /*gen*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "UserLostCB()";
}

void XN_CALLBACK_TYPE PoseStartCB(
   xn::PoseDetectionCapability& /*cap*/,
   const XnChar* /*pose*/,
   XnUserID id,
   void* cookie
)
{
   LOG(logDEBUG) << "PoseStartCB()";

   UserTrackerCallbackArgs* args = static_cast<UserTrackerCallbackArgs*>(cookie);
   args->user_gen->GetPoseDetectionCap().StopPoseDetection(id);
   args->user_gen->GetSkeletonCap().RequestCalibration(id, TRUE);
}

void XN_CALLBACK_TYPE PoseEndCB(
   xn::PoseDetectionCapability& /*cap*/,
   const XnChar* /*pose*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "PoseEndCB()";
}

void XN_CALLBACK_TYPE CalibStartCB(
   xn::SkeletonCapability& /*cap*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "CalibStartCB()";
}

void XN_CALLBACK_TYPE CalibEndCB(
   xn::SkeletonCapability& /*cap*/,
   XnUserID id,
   XnBool success,
   void* cookie
)
{
   UserTrackerCallbackArgs* args = static_cast<UserTrackerCallbackArgs*>(cookie);

   if (success)
   {
      LOG(logDEBUG) << "User successfully calibrated.";
      args->user_gen->GetSkeletonCap().StartTracking(id);
   }
   else
   {
      LOG(logDEBUG) << "Failed to calibrate user!";
      args->user_gen->GetPoseDetectionCap().StartPoseDetection("Psi", id);
   }
}

} // namespace kinectlib
