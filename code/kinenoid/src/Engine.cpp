#include "Engine.h"
#include <Windows.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#include "../res/resource.h"
#endif


//-------------------------------------------------------------------------------------
Engine::Engine(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
Engine::~Engine(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool Engine::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "Kinenoid");

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void Engine::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"Graupner");
}
//-------------------------------------------------------------------------------------
void Engine::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,100,-450));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,400));
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void Engine::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

}
//-------------------------------------------------------------------------------------
void Engine::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void Engine::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void Engine::setupResources(void)
{
	
	// Only add the minimally required resource locations to load up SDKTrays mesh
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("C:/Users/Orri/Desktop/Projektarbeit/Repository/kinenoid/trunc/Engine/OgreSDK_vc10_v1-7-3/media/packs/SdkTrays.zip", "Zip", "General");
   Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../../external/OgreSDK_vc10_v1-7-3/media/materials/programs/", "FileSystem", "General");
//    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../../external/OgreSDK_vc10_v1-7-3/media/materials/scripts/", "FileSystem", "General");
   Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../../external/OgreSDK_vc10_v1-7-3/media/materials/textures/", "FileSystem", "General");
   Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../../external/OgreSDK_vc10_v1-7-3/media/models/", "FileSystem", "General");
}
//-------------------------------------------------------------------------------------
void Engine::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void Engine::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void Engine::go(void)
{
    if (!setup())
		return;
}
//-------------------------------------------------------------------------------------
bool Engine::setup(void)
{
    mRoot = new Ogre::Root("","");

	setupResources();
	loadPlugins();
	loadResources();

	bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
	return true;
};
//-------------------------------------------------------------------------------------

//Adjust mouse clipping area
void Engine::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void Engine::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

//-------------------------------------------------------------------------------------

void Engine::loadPlugins()
{
	// A list of required plugins
	Ogre::StringVector required_plugins;
	required_plugins.push_back("GL RenderSystem");
	required_plugins.push_back("Octree & Terrain Scene Manager");

	// List of plugins to load
	Ogre::StringVector plugins_toLoad;
	plugins_toLoad.push_back("RenderSystem_GL");
	plugins_toLoad.push_back("Plugin_OctreeSceneManager");

	// Load the OpenGL RenderSystem and the Octree SceneManager plugins
	for (Ogre::StringVector::iterator j = plugins_toLoad.begin(); j != plugins_toLoad.end(); j++)
	{
		#ifdef _DEBUG
		mRoot->loadPlugin(*j + Ogre::String("_d"));
		#else
		mRoot->loadPlugin(*j);
		#endif;
	}
	// Check if the required plugins are installed and ready for use
	// If not: exit the application
	Ogre::Root::PluginInstanceList ip = mRoot->getInstalledPlugins();
}
