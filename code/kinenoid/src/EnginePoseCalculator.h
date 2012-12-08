#pragma once

#include <XnTypes.h>
#include <irrlicht.h>
#include "Log.h"

using namespace irr;
using namespace core;
class EnginePoseCalculator
{
public:
	EnginePoseCalculator(void);
	~EnginePoseCalculator(void);

	vector3df getRotationArm(const vector3df& newVec, const vector3df& oldVec);
	vector3df getRotationLeftHand(const XnPoint3D& first, const XnPoint3D& second);
	vector3df getRotationLeftShoulder(const XnPoint3D& first, const XnPoint3D& second);

	vector3df getRotationRightArm(const XnPoint3D& first, const XnPoint3D& second);
	vector3df getRotationRightHand(const XnPoint3D& first, const XnPoint3D& second);
	vector3df getRotationRightShoulder(const XnPoint3D& first, const XnPoint3D& second);


	int calcAngle(vector3df first, vector3df second);
	float dotProdukt(vector3df first, vector3df second );
	float absVector(vector3df first);
	vector3df calcAngles( matrix4 rotMatrix );
private:

	quaternion quat;
};

