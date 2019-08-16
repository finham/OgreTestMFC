#pragma once
#include "ogre.h"
#include "OgreConfigFile.h"
#include "OgreFrameListener.h"
#include "OgreStringConverter.h"
#include "OIS.h"
#include "Ogre/SdkTrays.h"
#include "SdkTrays.h"

#include "MyOgreApp.h"
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include <deque> 
using namespace OgreBites;
using namespace std;
using namespace Ogre;
enum QueryFlags{
	NINJA_MASK = 1<<0, //NINJA模型
	ROBOT_MASK = 1<<1//ROBOT模型
};
class CMyFrameListener: public FrameListener, public OIS::MouseListener,public Ogre::WindowEventListener, public OIS::KeyListener//,public SdkTrayListener
{
public:
	CMyFrameListener(Ogre::SceneNode* node,RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light* light,Entity* ent,std::deque<Vector3> &walk);
	~CMyFrameListener(void);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	bool frameStarted(const Ogre::FrameEvent &evt);
	bool nextLocation();
	void onLeftPressed(const OIS::MouseEvent &arg);
protected:
	Ogre::SceneManager *mSceneMgr;//场景管理器
	Ogre::SceneNode* mCamNode;//摄像机节点
	Ogre::Camera* mCamera;//摄像机
	Ogre::RenderWindow* mWindow;//渲染窗口
	Ogre::Light* light;//灯光

	OIS::Keyboard* mKeyboard;//键盘         
	OIS::Mouse* mMouse;      //鼠标    
	OIS::InputManager* mInputManager;//输入管理器 

//	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::SdkCameraMan* mCameraMan;     // basic camera controller
	bool mRBtdown;
	Vector3 translate;
	float _movementspeed;
	
	//Real mDistance;                  // The distance the object has left to travel    
	//Vector3 mDirection;              // The direction the object is moving    
	//Vector3 mDestination;            // The destination the object is moving towards    

	Ogre::Entity *mEntity;                 // The Entity we are animating    
	Ogre::SceneNode *mNode;                // The SceneNode that the Entity is attached to    

	//std::deque<Vector3> mWalkList;   // The list of points we are walking to    

	float _rotation;
	float _WalkingSpeed; 
	RaySceneQuery *mRaySceneQuery;
	Ogre::AnimationState* _aniState;
	Ogre::AnimationState* _aniStateTop;
	Ogre::AnimationState* _aniStateDance;
	Ogre::AnimationState* _aniStateSH;
	Ogre::AnimationState* _aniStateSV;
	Ogre::AnimationState* _aniStateIB;
	Ogre::AnimationState* _aniStateIT;

	std::deque<Vector3> mWalkList;
	Real mDistance;
	Vector3 mDirection; 
	Vector3 mDestination;

	AnimationState *mAnimationState; 
	Real mWalkSpeed;
	Ogre::SceneNode* current_node;//生成的实体结点
	int m_count; //添加实体数量
	SdkTrayManager* mTrayMgr;
	bool mLMouseDown;
	bool mRobotMode;
	RaySceneQuery *mRaySceneQuery_mouse; // 射线场景查询指针
	CPoint point;
	CWnd* pWnd;
	//Label label;
};

