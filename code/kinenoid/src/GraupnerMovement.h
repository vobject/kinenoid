#ifndef __GRAUPNER_MOVEMENT_H__
#define __GRAUPNER_MOVEMENT_H__

namespace kinectlib {

typedef unsigned char GraupnerJointValue;

enum GraupnerJoint
{
   GJ_LEFT_HIPSIDE   =  1,
   GJ_LEFT_HIPUP     =  2,
   GJ_LEFT_HIPROT    =  3,
   GJ_LEFT_KNEE      =  4,
   GJ_LEFT_FOOT      =  5,
   GJ_LEFT_FOOTSIDE  =  6,
   GJ_LEFT_SHOULDER  =  7,
   GJ_LEFT_ARM       =  8,
   GJ_LEFT_HAND      =  9,
   GJ_RIGHT_HIPSIDE  = 10,
   GJ_RIGHT_HIPUP    = 11,
   GJ_RIGHT_HIPROT   = 12,
   GJ_RIGHT_KNEE     = 13,
   GJ_RIGHT_FOOT     = 14,
   GJ_RIGHT_FOOTSIDE = 15,
   GJ_RIGHT_SHOULDER = 16,
   GJ_RIGHT_ARM      = 17,
   GJ_RIGHT_HAND     = 18,
   GJ_HEAD           = 19,
   GRAUPNER_JOINT_ITEM_COUNT
};

class GraupnerMovement
{
public:
   static const int PROTOCOL_LENGTH = 21;

   GraupnerMovement();
   ~GraupnerMovement();

   static void GetInitPose(GraupnerJointValue protocol[]);
   static int GetInitJointPosition(GraupnerJoint joint);

   void GetCurrentPose(GraupnerJointValue protocol[]) const;
   void SetJointPosition(GraupnerJoint joint, int pos);
   void SetLED(bool enable);

   bool IsWalkInProgress() const;
   void NextAnimationstepForward();

private:
   float CalcAnimationDistance(int from, int to);
   void CalcDifferenceArray( const GraupnerJointValue from[], const GraupnerJointValue to[]);
   
   static const int WALK_ANIMATION_COUNT = 8;
   static const int MOVESTEPS = 16;
   static const GraupnerJointValue sInitPoseCommands[PROTOCOL_LENGTH];
   static const GraupnerJointValue sGraupnerWalkArray[WALK_ANIMATION_COUNT][PROTOCOL_LENGTH];

   int mAnimation_Position;
   int mAnimationSteps_Position;
   float mDifferenceArray[PROTOCOL_LENGTH];
   GraupnerJointValue mCommands[PROTOCOL_LENGTH];
};

} // namespace kinectlib

#endif // __GRAUPNER_MOVEMENT_H__
