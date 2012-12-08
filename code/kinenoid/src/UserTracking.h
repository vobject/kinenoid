#ifndef __USER_TRACKING_H__
#define __USER_TRACKING_H__

#include <vector>

#include <XnOS.h>
#include <XnCppWrapper.h>

#include "UserData.h"

namespace kinectlib {

// A pointer to this struct is passed as the "cookie" argument to the CB functions.
struct UserTrackerCallbackArgs
{
   UserTrackerCallbackArgs() : user_gen(NULL) { }

   xn::UserGenerator* user_gen;

private:
   UserTrackerCallbackArgs(const UserTrackerCallbackArgs&);
   UserTrackerCallbackArgs& operator=(const UserTrackerCallbackArgs&);
};

class UserTracking
{
public:
   UserTracking();
   ~UserTracking();

   void Init(xn::Context& ctx);
   void Shutdown();
   size_t GetUsers(std::vector<UserData>& users) const;

private:
   xn::UserGenerator mUserGen;

   XnCallbackHandle mUserGenCB;
   XnCallbackHandle mPoseCB;
   XnCallbackHandle mCalibCB;

   UserTrackerCallbackArgs mCallbackArgs;

   // Stuff we do not need atm.
   UserTracking(const UserTracking&);
   UserTracking& operator=(const UserTracking&);
};

// Callback functions for xn::UserGenerator class.
void XN_CALLBACK_TYPE UserFoundCB(xn::UserGenerator& gen, XnUserID id, void* cookie);
void XN_CALLBACK_TYPE UserLostCB(xn::UserGenerator& gen, XnUserID id, void* cookie);

// Callback functions for xn::PoseDetectionCapability class.
void XN_CALLBACK_TYPE PoseStartCB(xn::PoseDetectionCapability& gen, 
                                  const XnChar* pose, XnUserID id, void* cookie);
void XN_CALLBACK_TYPE PoseEndCB(xn::PoseDetectionCapability& gen, 
                                const XnChar* pose, XnUserID id, void* cookie);

// Callback functions for xn::SkeletonCapability class.
void XN_CALLBACK_TYPE CalibStartCB(xn::SkeletonCapability& gen, 
                                   XnUserID id, void* cookie);
void XN_CALLBACK_TYPE CalibEndCB(xn::SkeletonCapability& gen, XnUserID id,
                                 XnBool success, void* cookie);

} // namespace kinectlib

#endif // __USER_TRACKING_H__
