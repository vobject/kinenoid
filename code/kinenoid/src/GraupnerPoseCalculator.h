#ifndef __GRAUPNER_POSE_CALCULATOR_H__
#define __GRAUPNER_POSE_CALCULATOR_H__

#include <XnTypes.h>

#include "GraupnerMovement.h"

namespace kinectlib {

class GraupnerPoseCalculator
{
public:
   static void LeftShoulder(GraupnerMovement& movment,
                            const XnPoint3D& shoulder,
                            const XnPoint3D& elbow);
   static void LeftArm(GraupnerMovement& movment,
                       const XnPoint3D& shoulder,
                       const XnPoint3D& elbow);
   static void LeftHand(GraupnerMovement& movment,
                       const XnPoint3D& shoulder,
                       const XnPoint3D& elbow,
                       const XnPoint3D& hand);
   static void RightShoulder(GraupnerMovement& movment,
                       const XnPoint3D& shoulder,
                       const XnPoint3D& elbow);
   static void RightArm(GraupnerMovement& movment,
                       const XnPoint3D& shoulder,
                       const XnPoint3D& elbow);
   static void RightHand(GraupnerMovement& movment,
                       const XnPoint3D& shoulder,
                       const XnPoint3D& elbow,
                       const XnPoint3D& hand);
};

} // namespace kinectlib

#endif // __GRAUPNER_POSE_CALCULATOR_H__
