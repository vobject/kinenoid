#include "GraupnerPositionSmoothing.h"

#include <algorithm>

namespace kinectlib {

GraupnerPositionSmoothing::GraupnerPositionSmoothing()
{

}

GraupnerPositionSmoothing::~GraupnerPositionSmoothing()
{

}

void GraupnerPositionSmoothing::AddJointPosition( const GraupnerJoint joint, const GraupnerJointValue pos )
{
   mJointData[joint].push_back(pos);
}

void GraupnerPositionSmoothing::GetSmoothPose( GraupnerJointValue protocol[], const bool reset/*=true*/ )
{
   GraupnerMovement::GetInitPose(protocol);

   std::map<GraupnerJoint, std::vector<GraupnerJointValue> >::iterator joint_iter = mJointData.begin();
   std::map<GraupnerJoint, std::vector<GraupnerJointValue> >::iterator joint_end = mJointData.end();

   for (; joint_iter != joint_end; ++joint_iter)
   {
//      std::sort(joint_iter->second.begin(), joint_iter->second.end());
//      const GraupnerJointValue median_val = joint_iter->second.at(joint_iter->second.size() / 2);

      protocol[joint_iter->first] = (joint_iter->second[0]+joint_iter->second[joint_iter->second.size()/2]+joint_iter->second[joint_iter->second.size()-1])/3;
      joint_iter->second.clear();
   }

   if (reset)
   {
      //mJointData.clear();
   }
}

} // namespace kinectlib
