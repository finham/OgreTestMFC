#include "StdAfx.h"
#include "MyOgreApp.h"
#include <winuser.h>


CMyOgreApp::CMyOgreApp(void)
{
}


CMyOgreApp::~CMyOgreApp(void)
{
}


bool CMyOgreApp::go(CRect rt, HWND hWnd)
{
	createRoot();
	setupResources();
	setupRenderSystem();
	createRenderWindow(hWnd, rt.Width(), rt.Height());
	chooseSceneManager();
	createCamera();
	createViewport();
	initializeResourceGroups();
	createScene();
	createFrameListener();//创建侦听
	return true;
}


void CMyOgreApp::createRoot(void)
{
	mRoot = new Ogre::Root("plugins_d.cfg");
}


void CMyOgreApp::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("resources_d.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}


void CMyOgreApp::setupRenderSystem(void)
{
	mRoot->showConfigDialog();
	//if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
	//	throw Ogre::Exception(1, "未选择渲染系统","setupRenderSystem()");
}


void CMyOgreApp::createRenderWindow(HWND m_hWnd, int width, int height)
{
	mRoot->initialise(false);//禁止ogre创建新的渲染窗口
	NameValuePairList misc;
	misc["externalWindowHandle"] = StringConverter::toString((int)m_hWnd); 
	mWindow = mRoot->createRenderWindow("OgreRenderWindow", width, height, false, &misc);
}


int CMyOgreApp::initializeResourceGroups(void)
{
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	return 0;
}


void CMyOgreApp::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
}


void CMyOgreApp::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("MainCamera");

	// Position it at 500 in Z direction
	mCamera->setPosition(200,250,300);
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,-100,-300));
	mCamera->setNearClipDistance(5);
//	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}


void CMyOgreApp::createViewport(void)
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}


void CMyOgreApp::createScene(void)
{

	////添加背景
	//mSceneMgr->setSkyDome(true, "Examples/CloudySky",6, 8);

	//Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

	//Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//headNode->attachObject(ogreHead);

	//// Set ambient light
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//// Create a light
	//Ogre::Light* l = mSceneMgr->createLight("MainLight");
	//l->setPosition(20,80,50);

	Ogre::Plane plane(Vector3::UNIT_Y, -5);
	Ogre::MeshManager::getSingleton().createPlane("plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, TRUE, 1, 5, 5, Vector3::UNIT_Z);
	Ogre::Entity* ent1 = mSceneMgr->createEntity("LightPlaneEntity", "plane");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent1);
	ent1->setMaterialName("Examples/BeachStones");

	mSceneMgr->setAmbientLight(ColourValue(0, 0, 0)); 
	//添加灯光
	Ogre::Light* light =mSceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(1,-1,0));
	light->setDiffuseColour(1.0, 0.0, 0.0);
	light->setSpecularColour(1.0, 1.0,1.0);
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	mSceneMgr->setWorldGeometry("terrain.cfg");

	mSceneMgr->setAmbientLight( ColourValue( 1.0f, 1.0f, 1.0f ) );
	mEntity = mSceneMgr->createEntity( "Robot", "Sinbad.mesh" );//Robot Sinbad
	mNode = mSceneMgr->getRootSceneNode( )-> createChildSceneNode( "RobotNode", Vector3( 0.0f, 0.0f, 25.0f ) );
	mNode->attachObject( mEntity );
	mNode->setScale(10,10,10);
	mNode->setPosition(200,200,50);
	
	mWalkList.push_back( Vector3( 550.0f, 0.0f, 100.0f ) ); 
	mWalkList.push_back( Vector3(700.0f, 0.0f, 300.0f ) );
	mWalkList.push_back( Vector3(1000.0f, 0.0f, 500.0f ) );
	Entity *ent; 
	SceneNode *node;
	ent = mSceneMgr->createEntity( "Knot1", "knot.mesh" );
	node = mSceneMgr->getRootSceneNode( )->createChildSceneNode( "Knot1Node", Vector3( 0.0f, -10.0f, 25.0f ) );
	node->attachObject( ent );
	node->setScale( 0.1f, 0.1f, 0.1f );

	ent = mSceneMgr->createEntity( "Knot2", "knot.mesh" );
	node = mSceneMgr->getRootSceneNode( )->createChildSceneNode( "Knot2Node", Vector3( 550.0f, -10.0f, 50.0f ) );
	node->attachObject( ent );
	node->setScale( 0.1f, 0.1f, 0.1f );

	ent = mSceneMgr->createEntity( "Knot3", "knot.mesh" );
	node = mSceneMgr->getRootSceneNode( )->createChildSceneNode( "Knot3Node", Vector3(-100.0f, -10.0f,-200.0f ) );
	node->attachObject( ent ); 
	node->setScale( 0.1f, 0.1f, 0.1f );

	Entity *ent2; 
	SceneNode *node1;
	ent2 = mSceneMgr->createEntity("tooth","bird.mesh");
	node1 = mSceneMgr->getRootSceneNode( )->createChildSceneNode( "ToothNode", Vector3(-100.0f, -10.0f,-200.0f ) );
	node1->attachObject( ent2 ); 
	node1->setScale( 0.3f, 0.3f, 0.3f );
	node1->setPosition(100,86,10);
}


void CMyOgreApp::createFrameListener(void)
{
	mListener= new CMyFrameListener(mNode,mWindow, mCamera, mSceneMgr,light,mEntity,mWalkList);
	mRoot->addFrameListener(mListener);
}
