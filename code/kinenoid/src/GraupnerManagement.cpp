#include "GraupnerManagement.h"

#include "Log.h"
#include "UserData.h"

namespace kinectlib {

GraupnerManagement::GraupnerManagement(KinectWrapper* subject, const std::string& port)
   : mSubject(subject)
   , mComPort(port)
   , mInitialset(false)
{
   mSubject->Subscribe(this);
}

GraupnerManagement::~GraupnerManagement()
{
   if (mSubject) {
      mSubject->Unsubscribe(this);
   }
}

void GraupnerManagement::Update( const Subject* updated_subject )
{
   if (updated_subject != mSubject) {
      // This is not the subject that we registered to.
      throw "Invalid subject.";
   }

   if (IsConnected())
   {
      MoveRobotJoints();

      GraupnerJointValue cmd[GraupnerMovement::PROTOCOL_LENGTH];
      mMovement.GetCurrentPose(cmd);
      mConnection.Write(cmd, GraupnerMovement::PROTOCOL_LENGTH);
   }
}

void GraupnerManagement::Init(const bool send_init_pose /*=true*/)
{
   mInitialset = false;
   if (!mConnection.Connect(mComPort))
   {
      LOG(logDEBUG) << "Connection to Graupner could not be established.";
   }
   else
   {
      LOG(logDEBUG) << "Connection to Graupner established.";

      if (send_init_pose)
      {
         GraupnerJointValue cmd[GraupnerMovement::PROTOCOL_LENGTH];
         GraupnerMovement::GetInitPose(cmd);
         mConnection.Write(cmd, GraupnerMovement::PROTOCOL_LENGTH);
      }
   }
}

void GraupnerManagement::Shutdown()
{
   mConnection.Disconnect();
}

bool GraupnerManagement::IsConnected() const
{
   return mConnection.IsConnected();
}

void GraupnerManagement::MoveRobotJoints()
{
   std::vector<UserData> users;
   if (!mSubject->GetUsers(users)) {
      // We need at least one tracked user.
      return;
   }

   // We currently support only one tracked user for Graupner robot.
   std::map<int, XnPoint3D> joints = users[0].GetRealWorldJoints();

   if ( !mInitialset )
   {
      mDistance = joints[XN_SKEL_TORSO];
      mInitialset = true;
   }

   if (mMovement.IsWalkInProgress())//if walk one step not ready next animation
   {
      mMovement.NextAnimationstepForward();//trigger next animation
      return;//end method
   }

   XnFloat distance_now = mDistance.Z - joints[XN_SKEL_TORSO].Z;//calc distance between aktual torso point to saved torso point
   if ( distance_now > 200.00)// if distance_now>200 walk_animation triggert
   {
      mMovement.NextAnimationstepForward();//trigger walk animation
      mDistance = joints[XN_SKEL_TORSO];//reset saved torso point
      
      return;//end method
   }

   if ( distance_now < -200.00)// if distance_now>200 walk_animation triggert
   {
      mDistance = joints[XN_SKEL_TORSO];//reset saved torso point
      
      return;//end method
   }
   /*if ( mInitialset && (distance_now < 400.00)
   {
      mPoseCalc.walkback();
      mDistance = joints[XN_SKEL_TORSO];
   }
   */

   // Move left shoulder
   mPoseCalc.LeftShoulder(mMovement,
                     joints[XN_SKEL_LEFT_SHOULDER],
                     joints[XN_SKEL_LEFT_ELBOW]);
   // Move left arm
   mPoseCalc.LeftArm(mMovement,
                     joints[XN_SKEL_LEFT_SHOULDER],
                     joints[XN_SKEL_LEFT_ELBOW]);
   //Move left hand
   mPoseCalc.LeftHand(mMovement,
                     joints[XN_SKEL_LEFT_SHOULDER],
                     joints[XN_SKEL_LEFT_ELBOW],
                     joints[XN_SKEL_LEFT_HAND]);
   // Move right shoulder
   mPoseCalc.RightShoulder(mMovement,
                     joints[XN_SKEL_RIGHT_SHOULDER],
                     joints[XN_SKEL_RIGHT_ELBOW]);
   // Move right arm
   mPoseCalc.RightArm(mMovement,
                     joints[XN_SKEL_RIGHT_SHOULDER],
                     joints[XN_SKEL_RIGHT_ELBOW]);
   //Move right hand
   mPoseCalc.RightHand(mMovement,
      joints[XN_SKEL_RIGHT_SHOULDER],
      joints[XN_SKEL_RIGHT_ELBOW],
      joints[XN_SKEL_RIGHT_HAND]);
   //problem mit der raumkoordinate überprüfen was passiert wenn oberkörper sich neigt oder dreht
   //kniebeuge,laufen über led/laser byte
}

} // namespace kinectlib
