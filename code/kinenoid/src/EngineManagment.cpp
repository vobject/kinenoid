#include "EngineManagment.h"
#include <XnTypes.h>


EngineManagment::EngineManagment(kinectlib::KinectWrapper* subject) : mSubject(subject)
{
	mSubject->Subscribe(this);
	oldVectorRight = new Ogre::Vector3(0,0,0);
	oldVectorLeft = new Ogre::Vector3(0,0,0);
}

EngineManagment::~EngineManagment(void)
{
	if(mSubject)
		mSubject->Unsubscribe(this);
   Shutdown();
}

void EngineManagment::createScene(void){
	//Create Ninja Model (3th Party)
	ninja = mSceneMgr->createEntity("Ninja", "ninja.mesh");

	// Create Graupner Model
	//graupnerModel = mSceneMgr->createEntity("Graupner", "Graupner.mesh");
	// Add Model to Scene
	//Ogre::SceneNode* graupnerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//graupnerNode->attachObject(graupnerModel);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* lef = mSceneMgr->createLight("MainLight");
	lef->setPosition(20,80,50);
	//Attach to Scene
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninja);
	if(ninja->hasSkeleton())
	{
		Ogre::Skeleton *skeleton = ninja->getSkeleton();
		skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

		leftBone = skeleton->getBone("Joint10"); //Linke Schulter
		leftBone->setManuallyControlled(true);		

		rightBone = skeleton->getBone("Joint15"); //rechte Schulter
		rightBone->setManuallyControlled(true);
	}
	leftBone->setOrientation(0, -1, 1,0);
	rightBone->setOrientation(0, -1, -1, 0);
}

void EngineManagment::drawAll(void){
	 //render scene
	mRoot->renderOneFrame();
	//mRoot->startRendering();
	 // clean up
	 destroyScene();
}

void EngineManagment::Update(const kinectlib::Subject* updated_subject ){
   
	std::vector<kinectlib::UserData> users;
	mSubject->GetUsers(users);
	if(users.empty() || !mRoot)
		return;
	std::map<int, XnPoint3D> joints = users[0].GetRealWorldJoints();
	XnPoint3D elbowRight = joints[XN_SKEL_RIGHT_ELBOW];
	XnPoint3D shoulderRight = joints[XN_SKEL_RIGHT_SHOULDER];
	XnPoint3D elbowLeft = joints[XN_SKEL_LEFT_ELBOW];
	XnPoint3D shoulderLeft = joints[XN_SKEL_LEFT_SHOULDER];

	setBoneOrientation(rightBone, elbowLeft, shoulderLeft, oldVectorLeft);
	setBoneOrientation(leftBone, elbowRight, shoulderRight, oldVectorRight);

	oldVectorRight = createVector(elbowRight, shoulderRight); //Wird beim nächsten Aufruf 
	oldVectorLeft = createVector(elbowLeft, shoulderLeft);    //als Quelle benutzt
}

void EngineManagment::setBoneOrientation(Ogre::Bone *bone, XnPoint3D first, XnPoint3D second, Ogre::Vector3 *oldVector)
{
	Ogre::Quaternion quantInit = bone->getInitialOrientation();
	Ogre::Vector3 *newVector = createVector(first, second);
	Ogre::Quaternion quantResult = oldVector->getRotationTo(*newVector);
	Ogre::Quaternion boneOrient = bone->getOrientation();
	bone->setOrientation(quantResult*boneOrient*quantInit);
}

Ogre::Vector3 * EngineManagment::createVector( XnPoint3D elbow, XnPoint3D shoulder )
{
	return new Ogre::Vector3(shoulder.X - elbow.X, shoulder.Y - elbow.Y, shoulder.Z - elbow.Z);
}

bool EngineManagment::isInitialized()
{
	return (mRoot != NULL);
}

void EngineManagment::Shutdown()
{
   if(mRoot) {
      mSceneMgr->clearScene();
      mRoot->destroySceneManager(mSceneMgr);
      mWindow->destroy();
      mRoot->shutdown();
      delete mRoot;
      mRoot = NULL;
   }
}

void EngineManagment::Init()
{
   if(mRoot)
      return;
	go();  //Starte Engine
	createScene(); //Erzeuge Scene (sichbarer Bereich)
}

