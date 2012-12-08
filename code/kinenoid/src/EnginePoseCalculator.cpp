#include "EnginePoseCalculator.h"
#include <quaternion.h>
#include <cmath>
#include <matrix4.h>

using namespace irr;
using namespace core;

EnginePoseCalculator::EnginePoseCalculator(void):quat()
{

}


EnginePoseCalculator::~EnginePoseCalculator(void)
{
}

//************************************
// Method:    getRotationArm
// FullName:  EnginePoseCalculator::getRotationArm
// Access:    public 
// Returns:   Vector(x,y,z) X Drehung Links/Rechts; Y Drehung Hoch/Runter; Z Drehung um die eigene Achse
// Qualifier:
// Parameter: const XnPoint3D & shoulder Vektor mit XYZ-Koordinaten
// Parameter: const XnPoint3D & elbow Vektor mit XYZ-Koordinaten
//************************************
//LINKE HAND REGEL
//			 |y   
//			 |  /z
//			 | /
//			 |/____ x
//************************************			  
vector3df EnginePoseCalculator::getRotationArm(const vector3df& newVec, const vector3df& oldVec){

		 quat.rotationFromTo(oldVec, newVec);
		 matrix4 rotMatrix = quat.getMatrix();
		 return rotMatrix.getRotationDegrees();

}
vector3df EnginePoseCalculator::getRotationLeftHand(const XnPoint3D& first, const XnPoint3D& second){


	return vector3df(0,0,0);
}
vector3df EnginePoseCalculator::getRotationLeftShoulder(const XnPoint3D& first, const XnPoint3D& second){
	
	return vector3df(0,0,0);
}


///HELPERS
float EnginePoseCalculator::dotProdukt( vector3df first, vector3df second ) 
{
	return (first.X*second.X) + (first.Y*second.Y) + (first.Z*second.Z);
}

float EnginePoseCalculator::absVector(vector3df first) 
{
	return sqrtf(powf(first.X, 2) + powf(first.Y, 2) + powf(first.Z, 2));
}

int EnginePoseCalculator::calcAngle(irr::core::vector3df first, irr::core::vector3df second){

	float denominator = dotProdukt(first, second);
	float numerator = absVector(first) * absVector(second);
	return (int) (180.0 / (3.1415926535897)) * acosf(denominator / numerator);

}

irr::core::vector3df EnginePoseCalculator::calcAngles(matrix4 rotMatrix )
{
	int yAngle = -asinf(rotMatrix[2]);
	int C = cosf(yAngle);
	yAngle *= RADTODEG; //glaub radtodeg //RADIANS

	if(abs(C) > 0,005){
		int tr_x = rotMatrix[10] /C;
		int tr_y = -rotMatrix[6] /C;

		int xAngle = atan2f(tr_y,tr_x) * RADTODEG;

		tr_y = rotMatrix[0] /C;
		tr_x = rotMatrix[1] /C;

		int zAngle = atan2f(tr_y, tr_x) * RADTODEG;
		return vector3df(xAngle, yAngle, zAngle);
	}else{
		int xAngle = 0;
		int tr_y = rotMatrix[5];
		int tr_x = rotMatrix[4];
		int zAngle = atan2f(tr_y, tr_x) * RADTODEG;
		return vector3df(xAngle, yAngle, zAngle);
	}
  return vector3df(0,0,0);
}
