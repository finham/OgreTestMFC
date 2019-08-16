#pragma once
#include "ogre.h"
#include "OgreConfigFile.h"
#include "OgreFrameListener.h"
#include "OgreStringConverter.h"
#include "OIS.h"

#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include "MyFrameListener.h"
using namespace Ogre;
class CMyOgreApp
{
public:
	CMyOgreApp(void);
	~CMyOgreApp(void);

	Ogre::Root* getRoot(void) const {return mRoot;}
	Ogre::Camera* getCamera(void) const {return mCamera;}
	Ogre::SceneNode* getCamNode(void) const {return mCamNode;}
	Ogre::SceneManager* getSceneManager(void) const {return mSceneMgr;}
	Ogre::RenderWindow* getRenderWindow(void) const {return mWindow;}

	bool go(CRect rt, HWND hWnd);
	void createRoot(void);
	void setupResources(void);
	void setupRenderSystem(void);
	void createRenderWindow(HWND m_hWnd, int width, int height);
	int initializeResourceGroups(void);
	void chooseSceneManager(void);
	void createCamera(void);
	void createViewport(void);
	void createScene(void);
	void createFrameListener(void);

private:
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;            
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCamNode;
	OgreBites::SdkCameraMan* mCameraMan;     // basic camera controller
	class CMyFrameListener* mListener;
	Ogre::Light* light;
	Ogre::Entity *mEntity; 
	Ogre::SceneNode *mNode;
	std::deque<Vector3> mWalkList;
};




