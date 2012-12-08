#ifndef __USER_DATA_H__
#define __USER_DATA_H__

#include <map>
#include <string>

#include <XnTypes.h>

namespace kinectlib {

class UserData
{
public:
	UserData(XnUserID id);
	virtual ~UserData();

   XnUserID GetId() const;

   std::map<int, XnPoint3D> GetRealWorldJoints() const;
   void SetRealWorldJoints(int joint, const XnPoint3D& pos);

   std::string JointId2JointName(int joint_id) const;

protected:
	XnUserID mId;
   std::map<int, XnPoint3D> mRealWorldJoints;

private:
   mutable std::map<int, std::string> mJointNames;
};

} // namespace kinectlib

#endif // __USER_DATA_H__
