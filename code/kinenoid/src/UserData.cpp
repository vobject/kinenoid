#include "UserData.h"

#include <XnCppWrapper.h>

namespace kinectlib {

UserData::UserData(XnUserID id)
   : mId(id)
{
   // TODO: This lookup table shout be outsourced.
   mJointNames[XN_SKEL_HEAD] = "HEAD";
   mJointNames[XN_SKEL_NECK] = "NECK";
   mJointNames[XN_SKEL_TORSO] = "TORSO";
   mJointNames[XN_SKEL_WAIST] = "WAIST";

   mJointNames[XN_SKEL_LEFT_COLLAR] = "LEFT_COLLAR";
   mJointNames[XN_SKEL_LEFT_SHOULDER] = "LEFT_SHOULDER";
   mJointNames[XN_SKEL_LEFT_ELBOW] = "LEFT_ELBOW";
   mJointNames[XN_SKEL_LEFT_WRIST] = "LEFT_WRIST";
   mJointNames[XN_SKEL_LEFT_HAND] = "LEFT_HAND";
   mJointNames[XN_SKEL_LEFT_FINGERTIP] = "LEFT_FINGERTIP";

   mJointNames[XN_SKEL_RIGHT_COLLAR] = "RIGHT_COLLAR";
   mJointNames[XN_SKEL_RIGHT_SHOULDER] = "RIGHT_SHOULDER";
   mJointNames[XN_SKEL_RIGHT_ELBOW] = "RIGHT_ELBOW";
   mJointNames[XN_SKEL_RIGHT_WRIST] = "RIGHT_WRIST";
   mJointNames[XN_SKEL_RIGHT_HAND] = "RIGHT_HAND";
   mJointNames[XN_SKEL_RIGHT_FINGERTIP] = "RIGHT_FINGERTIP";

   mJointNames[XN_SKEL_LEFT_HIP] = "LEFT_HIP";
   mJointNames[XN_SKEL_LEFT_KNEE] = "LEFT_KNEE";
   mJointNames[XN_SKEL_LEFT_ANKLE] = "LEFT_ANKLE";
   mJointNames[XN_SKEL_LEFT_FOOT] = "LEFT_FOOT";

   mJointNames[XN_SKEL_RIGHT_HIP] = "RIGHT_HIP";
   mJointNames[XN_SKEL_RIGHT_KNEE] = "RIGHT_KNEE";
   mJointNames[XN_SKEL_RIGHT_ANKLE] = "RIGHT_ANKLE";
   mJointNames[XN_SKEL_RIGHT_FOOT] = "RIGHT_FOOT";
}

UserData::~UserData()
{

}

XnUserID UserData::GetId() const
{
   return mId;
}

std::map<int, XnPoint3D> UserData::GetRealWorldJoints() const
{
   return mRealWorldJoints;
}

void UserData::SetRealWorldJoints( const int joint, const XnPoint3D& pos )
{
   mRealWorldJoints[joint] = pos;
}

std::string UserData::JointId2JointName(const int joint_id) const
{
   return mJointNames[joint_id];
}

} // namespace kinectlib
