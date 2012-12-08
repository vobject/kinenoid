#include "GraupnerMovement.h"

#include <cstring>

namespace kinectlib {

// Initial pose definition.
const GraupnerJointValue GraupnerMovement::sInitPoseCommands[PROTOCOL_LENGTH] = {
   255,  // Protocollstart
   124,  // Servo  #1 - GJ_LEFT_HIPSIDE
   130,  // Servo  #2 - GJ_LEFT_HIPUP
   145,  // Servo  #3 - GJ_LEFT_HIPROT
    20,  // Servo  #4 - GJ_LEFT_KNEE
   140,  // Servo  #5 - GJ_LEFT_FOOT
   140,  // Servo  #6 - GJ_LEFT_FOOTSIDE
   150,  // Servo  #7 - GJ_LEFT_SHOULDER
   120,  // Servo  #8 - GJ_LEFT_ARM
   135,  // Servo  #9 - GJ_LEFT_HAND
   124,  // Servo #10 - GJ_RIGHT_HIPSIDE
   125,  // Servo #11 - GJ_RIGHT_HIPUP
   140,  // Servo #12 - GJ_RIGHT_HIPROT
   254,  // Servo #13 - GJ_RIGHT_KNEE
   110,  // Servo #14 - GJ_RIGHT_FOOT
   132,  // Servo #15 - GJ_RIGHT_FOOTSIDE
   120,  // Servo #16 - GJ_RIGHT_SHOULDER
   128,  // Servo #17 - GJ_RIGHT_ARM
   125,  // Servo #18 - GJ_RIGHT_HAND
   128,  // Servo #19 - GJ_HEAD
     0   // LED off
};

//Definition for the Walk Animations
const GraupnerJointValue GraupnerMovement::sGraupnerWalkArray[WALK_ANIMATION_COUNT][PROTOCOL_LENGTH] =
{ 
   {255, 124,	130,	145,	20,	140,	140,	150,	120,	135,		124,	125,	140,	254,	110,	132,	120,	128,	125,		128, 0},
   {255, 134,	130,	145,	20,	140,	130,	150,	120,	135,		124,	125,	140,	254,	110,	112,	120,	128,	125,		128, 0},
   {255, 134,	150,	145,	20,	154,	130,	150,	120,	135,		124,	135,	140,	254,	120,	118,	120,	128,	125,		128, 0},
   {255, 124,	150,	145,	20,	154,	170,	150,	120,	135,		114,	135,	140,	254,	120,	132,	120,	128,	125,		128, 0},
   {255, 124,	120,	145,	20,	130,	160,	150,	120,	135,		114,	105,	140,	254,	130,	122,	120,	128,	125,		128, 0},
   {255, 134,	120,	145,	20,	130,	140,	150,	120,	135,		124,	115,	140,	254,	 90,	118,	120,	128,	125,		128, 0},
   {255, 134,	150,	145,	20,	154,	130,	150,	120,	135,		124,	135,	140,	254,	120,	118,	120,	128,	125,		128, 0},
   {255, 124,	130,	145,	20,	140,	140,	150,	120,	135,		124,	125,	140,	254,	110,	132,	120,	128,	125,		128, 0}
};


GraupnerMovement::GraupnerMovement()
{   
   GetInitPose(mCommands);
   mAnimation_Position = 0;
   mAnimationSteps_Position = 0;
   CalcDifferenceArray(sGraupnerWalkArray[mAnimation_Position], sGraupnerWalkArray[mAnimation_Position+1]);
}

GraupnerMovement::~GraupnerMovement()
{

}

void GraupnerMovement::GetInitPose( GraupnerJointValue protocol[] )
{
   memcpy(protocol, sInitPoseCommands, PROTOCOL_LENGTH);
}

int GraupnerMovement::GetInitJointPosition( const GraupnerJoint joint )
{
   return sInitPoseCommands[joint];
}

void GraupnerMovement::GetCurrentPose( GraupnerJointValue protocol[] ) const
{
   memcpy(protocol, mCommands, PROTOCOL_LENGTH);
}

void GraupnerMovement::SetJointPosition( const GraupnerJoint joint, const int pos )
{
   mCommands[joint] = static_cast<unsigned char>(pos);
}

void GraupnerMovement::SetLED( const bool enable )
{
   mCommands[20] = enable ? 1 : 0;
}

bool GraupnerMovement::IsWalkInProgress() const
{
   if((mAnimationSteps_Position > 0 && mAnimationSteps_Position < (MOVESTEPS - 1)) ||  //if walking & not done 15
      (mAnimation_Position > 0 && mAnimation_Position <= (WALK_ANIMATION_COUNT - 1)))     //if animating & not done 7 <= because 7 should be done also
      return true;
   return false;
}

float GraupnerMovement::CalcAnimationDistance(int from, int to)//calcs distance between 2 animationsteps of one servo
{      
      float diff;
		diff = to - from;
		diff = diff / MOVESTEPS;//movestepos anzahl der teilwerte
      return diff;      
}

void GraupnerMovement::CalcDifferenceArray( const GraupnerJointValue from[], const GraupnerJointValue to[])//makes array for distances
{
   for(int i = 0; i < PROTOCOL_LENGTH; i++)
   {
      mDifferenceArray[i] = CalcAnimationDistance(from[i], to[i]);
   }
}

void GraupnerMovement::NextAnimationstepForward()
{
   for(int i = 0; i < PROTOCOL_LENGTH; i++)
   {
      mCommands[i] = sGraupnerWalkArray[mAnimation_Position][i] + ( mAnimationSteps_Position * mDifferenceArray[i]);
      //add step_position* difference_array to actual animationpoint
   }
   mAnimationSteps_Position++;
   if(mAnimationSteps_Position > (MOVESTEPS - 1))//reached animationsteps
   { 
      mAnimationSteps_Position = 0;//reset animationsteps
      mAnimation_Position++;//next animation
      if(mAnimation_Position > (WALK_ANIMATION_COUNT - 1))//check if animations reached
         mAnimation_Position = 0;//reset animations
      if(mAnimation_Position==(WALK_ANIMATION_COUNT - 1))//last animation position should be triggert without animations
      {
         mAnimationSteps_Position= (MOVESTEPS - 1);//max should be reached
         CalcDifferenceArray(sGraupnerWalkArray[mAnimation_Position], sGraupnerWalkArray[mAnimation_Position]);//difference arry should be 0
      }
      else
      {
         CalcDifferenceArray(sGraupnerWalkArray[mAnimation_Position], sGraupnerWalkArray[mAnimation_Position+1]);
      }
   }
}

} // namespace kinectlib
