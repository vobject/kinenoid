#pragma once
#include "GraupnerManagement.h"
#include "Engine.h"

class EngineManagment : public kinectlib::Observer, public Engine
{
public:
	EngineManagment(kinectlib::KinectWrapper* subject);
	~EngineManagment(void);

	void drawAll();
	virtual void createScene();
	virtual void Update(const kinectlib::Subject* updated_subject);
	void setBoneOrientation( Ogre::Bone * rightArm, XnPoint3D elbow, XnPoint3D shoulder, Ogre::Vector3 *oldVector);
	Ogre::Vector3 * createVector( XnPoint3D elbow, XnPoint3D shoulder );
	bool isInitialized();
	void Shutdown();
	void Init();
private: 
	kinectlib::KinectWrapper* const mSubject;
	kinectlib::GraupnerMovement mMovement;
	kinectlib::GraupnerPoseCalculator mPoseCalc;
	Ogre::Entity* graupnerModel;
	Ogre::Entity* ninja;
	Ogre::Bone* leftBone;
	Ogre::Bone* rightBone;
	Ogre::Vector3 *oldVectorRight;
	Ogre::Vector3 *oldVectorLeft;
};
