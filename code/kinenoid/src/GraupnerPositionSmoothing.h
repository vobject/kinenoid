#ifndef __GRAUPNER_POSITION_SMOOTHING_H__
#define __GRAUPNER_POSITION_SMOOTHING_H__

#include <map>
#include <vector>

#include "GraupnerMovement.h"

namespace kinectlib {

class GraupnerPositionSmoothing
{
public:
   GraupnerPositionSmoothing();
   ~GraupnerPositionSmoothing();

   void AddJointPosition(GraupnerJoint joint, GraupnerJointValue pos);
   void GetSmoothPose(GraupnerJointValue protocol[], bool reset = true);

private:
   std::map<GraupnerJoint, std::vector<GraupnerJointValue> > mJointData;
};

} // namespace kinectlib

#endif // __GRAUPNER_POSITION_SMOOTHING_H__
