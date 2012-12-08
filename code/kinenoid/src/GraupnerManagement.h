#ifndef __GRAUPNER_MANAGEMENT_H__
#define __GRAUPNER_MANAGEMENT_H__

#if defined(WIN32)
#include <SDL.h>
#elif defined (__linux__)
#include <SDL/SDL.h>
#endif

#include "Observer.h"
#include "KinectWrapper.h"
#include "GraupnerConnection.h"
#include "GraupnerMovement.h"
#include "GraupnerPoseCalculator.h"

namespace kinectlib {

class GraupnerManagement : public Observer
{

public:
   explicit GraupnerManagement(KinectWrapper* subject, const std::string& port);
   virtual ~GraupnerManagement();

   virtual void Update(const Subject* updated_subject);

   void Init(bool send_init_pose = true);
   void Shutdown();
   bool IsConnected() const;

private:
   void MoveRobotJoints();

   KinectWrapper* const mSubject;
   const std::string mComPort;

   SDL_TimerID mSendTimer;
   GraupnerConnection mConnection;
   GraupnerMovement mMovement;
   GraupnerPoseCalculator mPoseCalc;
   bool mInitialset;
   XnPoint3D mDistance;

   GraupnerManagement& operator=(const GraupnerManagement&);
};

} // namespace kinectlib

#endif // __GRAUPNER_MANAGEMENT_H__
