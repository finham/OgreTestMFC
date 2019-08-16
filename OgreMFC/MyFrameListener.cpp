#include "StdAfx.h"
#include "MyFrameListener.h"
#include "Ogre/SdkTrays.h"
#include "SdkTrays.h"


CMyFrameListener::CMyFrameListener(Ogre::SceneNode* node,RenderWindow* win, Camera* cam, SceneManager *sceneMgr, Light *l,Entity *ent,std::deque<Vector3> &walk): mMouse(0), 
	mKeyboard(0), mInputManager(0), mWindow(win), mCamera(cam), light(l),mWalkList(walk)
{
	mRBtdown = false;
	mCamNode=cam->getParentSceneNode();
	mSceneMgr = sceneMgr;

	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);  

	windowHnd = (size_t )AfxGetMainWnd()->GetSafeHwnd(); // 这里这个窗口句柄就是传入的MFC主窗口
	windowHndStr << windowHnd;
	// OIS的窗口必须要顶层窗口,所以只有传MFC的主窗口给他,传view就不行
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	// 设置鼠标显示和非游戏独占,这样鼠标可以显示在屏幕上并可以移动到窗口外
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	// 键盘非游戏独占
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	mInputManager = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));//缓冲式方法实现
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mRaySceneQuery=sceneMgr->createRayQuery(Ray());

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	translate=(0,0,0);
	_movementspeed = 0.3;
	mCamera=cam;
	mNode=node;
	mEntity=ent;
	_rotation = 0.0f;
	_WalkingSpeed = 50.0f;
	mDirection = Vector3::ZERO;
	mWalkSpeed = 35.0f; 

	_aniState = mEntity->getAnimationState("RunBase"); //添加动作
	//_aniState->setEnabled(true); 
	_aniState->setLoop(false); 
	_aniStateTop = mEntity->getAnimationState("RunTop");
	//_aniStateTop->setEnabled(true);
	_aniStateTop->setLoop(false);
	_aniStateDance = mEntity->getAnimationState("Dance");
	_aniStateDance->setLoop(false);
	_aniStateSH = mEntity->getAnimationState("SliceHorizontal");
	_aniStateSH->setLoop(false);
	_aniStateSV = mEntity->getAnimationState("SliceVertical");
	_aniStateSV->setLoop(false);
	_aniStateIB = mEntity->getAnimationState("IdleBase");
	_aniStateIB->setLoop(false);
	_aniStateIT = mEntity->getAnimationState("IdleTop");
	_aniStateIT->setLoop(false);
	
	current_node =NULL;
	m_count =0;
	mTrayMgr =new SdkTrayManager("MyTray",win,mMouse,mTrayMgr);
	mTrayMgr->showFrameStats(TL_BOTTOMLEFT);//左下角放帧数据
	mTrayMgr->showLogo(TL_BOTTOMRIGHT);//右下角放logo
	mLMouseDown =false;
	mLMouseDown =false;
	mRaySceneQuery_mouse = mSceneMgr->createRayQuery(Ray());
	const OIS::MouseState &ms=mMouse->getMouseState();
	ms.height=win->getHeight();//不重写的话，默认活动范围是左上角50x50的框
	ms.width=win->getWidth();
	light=l;
}

bool CMyFrameListener::nextLocation(){
	if (mWalkList.empty())         
		return false;
	mDestination = mWalkList.front();       
	mWalkList.pop_front();
	mDirection = mDestination - mNode->getPosition();  
	mDirection.y=0;
	mDistance = mDirection.normalise();
	Vector3 src = mNode->getOrientation() * Vector3::UNIT_Z;        
	if ((1.0f + src.dotProduct(mDirection)) < 0.0001f){           
		mNode->pitch(Degree(180));        
	}        
	else{            
		Ogre::Quaternion quat = src.getRotationTo(mDirection);            
		mNode->rotate(quat);        
	} 
	return true;    
} 


CMyFrameListener::~CMyFrameListener(void)
{
	mInputManager->destroyInputObject(mMouse);
	mInputManager->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = 0;
}


bool CMyFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt){

	mMouse->capture();
	mKeyboard->capture();
	mCameraMan->frameRenderingQueued(evt); 
	_aniState->addTime(evt.timeSinceLastFrame);//修改值可以改变其动作速度
	//_Cam[0]->moveRelative(translate*evt.timeSinceLastFrame * _movementspeed);
	_aniStateTop->addTime(evt.timeSinceLastFrame);
	_aniStateDance->addTime(evt.timeSinceLastFrame);
	_aniStateSH->addTime(evt.timeSinceLastFrame);
	_aniStateSV->addTime(evt.timeSinceLastFrame);
	_aniStateIB->addTime(evt.timeSinceLastFrame);
	_aniStateIT->addTime(evt.timeSinceLastFrame);
	return true;
}

bool CMyFrameListener::mouseMoved(const OIS::MouseEvent &e)
{
	if (mRBtdown)
	{
		mCameraMan->injectMouseMove(e);
	}
	if (mLMouseDown){
		Ray mouseray =mTrayMgr->getCursorRay(mCamera);
		mRaySceneQuery_mouse->setRay(mouseray);
		mRaySceneQuery_mouse->setSortByDistance(false);
		RaySceneQueryResult &result_mouse = mRaySceneQuery_mouse->execute();//获取结果
		RaySceneQueryResult::iterator mouse_it = result_mouse.begin();//迭代器
		for(;mouse_it != result_mouse.end();mouse_it++){
			if (mouse_it->worldFragment)
			{
				current_node->setPosition(mouse_it->worldFragment->singleIntersection);
				break;
			} 
		}
	}
	mTrayMgr->injectMouseMove(e);//inject：触发
	return true;
}

bool CMyFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
//	mCameraMan->injectMouseDown(e, id);
	if (id == OIS::MB_Left)
	{
		onLeftPressed(e);
		mLMouseDown = true;
	}
	mTrayMgr->injectMouseDown(e,id);
	return true;
}

void CMyFrameListener::onLeftPressed(const OIS::MouseEvent &arg)
{
	if(current_node){
		current_node->showBoundingBox(false);
	}
	//创建射线查询
	Ray mouseray =mTrayMgr->getCursorRay(mCamera);
	mRaySceneQuery_mouse->setRay(mouseray);
	mRaySceneQuery_mouse->setSortByDistance(true);
	mRaySceneQuery_mouse->setQueryMask(mRobotMode ? ROBOT_MASK : NINJA_MASK);
	//开始查询
	RaySceneQueryResult &result_mouse = mRaySceneQuery_mouse->execute();
	RaySceneQueryResult::iterator mouse_it = result_mouse.begin();
	for(;mouse_it != result_mouse.end();mouse_it++){

		//检查第一个交点的是不是一个MovableObject，如果是，我们把它的父节点赋给mCurrentObject,并排除地形
		if (mouse_it->movable && mouse_it->movable->getName().substr(0, 5) != "tile[")
		{
			current_node = mouse_it->movable->getParentSceneNode();
			break;
		} 
		else if (mouse_it->worldFragment)
		{
			Entity *mouse_ent;
			char name[16];
			if(mRobotMode){
				sprintf(name,"robot%d",m_count++);
				mouse_ent = mSceneMgr->createEntity(name, "robot.mesh");
				mouse_ent->setQueryFlags(ROBOT_MASK);
			}else{
				sprintf(name, "Ninja%d", m_count++);
				mouse_ent = mSceneMgr->createEntity(name, "ninja.mesh");
				mouse_ent->setQueryFlags(NINJA_MASK);
			}
			current_node =mSceneMgr->getRootSceneNode()->createChildSceneNode(String(name)+"Node",mouse_it->worldFragment->singleIntersection);
			current_node->attachObject(mouse_ent);
			current_node->setScale(0.5f,0.5f,0.5f);
			/*DisplayString string=(DisplayString)current_node->getPosition();
			label.setCaption(current_node->getPosition);*/
			break;
		}
	}
	if(current_node){
		current_node->showBoundingBox(true);
	}
}

bool CMyFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{

//	mCameraMan->injectMouseUp(e, id);
	if (id == OIS::MB_Left)
	{
		onLeftPressed(e);
		mLMouseDown = false;
	}
	mTrayMgr->injectMouseUp(e,id);
	return true;
}
//键盘响应
bool CMyFrameListener::keyPressed(const OIS::KeyEvent &e)
{
	bool walked=false;
	//mCameraMan->injectKeyDown(e);
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		return false;
	}
	if(mKeyboard->isKeyDown(OIS::KC_W))
	{
		translate += Ogre::Vector3(0,0,-10);
		mCamera->moveRelative(Ogre::Vector3(0,0,-50) * _movementspeed);
	}
	if(mKeyboard->isKeyDown(OIS::KC_S))
	{
		translate += Ogre::Vector3(0,0,10);
		mCamera->moveRelative(Ogre::Vector3(0,0,50) * _movementspeed);
	}
	if(mKeyboard->isKeyDown(OIS::KC_A))
	{
		translate += Ogre::Vector3(-10,0,0);
		mCamera->moveRelative(Ogre::Vector3(-50,0,0) * _movementspeed);
	}
	if(mKeyboard->isKeyDown(OIS::KC_D))
	{
		translate += Ogre::Vector3(10,0,0);
		mCamera->moveRelative(Ogre::Vector3(50,0,0) * _movementspeed);
	}

	Ogre::Vector3 SinbadTranslate(0,0,0);
	if(mKeyboard->isKeyDown(OIS::KC_UP)) 
	{
		SinbadTranslate += Ogre::Vector3(0,0,-1);
		_rotation = 3.14f; 
		mNode->translate(SinbadTranslate * 1 * _WalkingSpeed); 
		mNode->resetOrientation(); //
		mNode->yaw(Ogre::Radian(_rotation));
		walked = true;
	}
	if(mKeyboard->isKeyDown(OIS::KC_DOWN)) 
	{ 
		SinbadTranslate += Ogre::Vector3(0,0,1); 
		_rotation = 0.0f; 
		mNode->translate(SinbadTranslate * 1 * _WalkingSpeed); 
		mNode->resetOrientation(); //
		mNode->yaw(Ogre::Radian(_rotation));
		walked = true; 
	} 
	if(mKeyboard->isKeyDown(OIS::KC_LEFT)) 
	{ 
		SinbadTranslate += Ogre::Vector3(-1,0,0); 
		_rotation = -1.57f; //-90
		mNode->translate(SinbadTranslate * 1 * _WalkingSpeed); 
		mNode->resetOrientation(); //
		mNode->yaw(Ogre::Radian(_rotation));
		walked = true; 
	}
	if(mKeyboard->isKeyDown(OIS::KC_RIGHT)) 
	{ 
		SinbadTranslate += Ogre::Vector3(1,0,0); 
		_rotation = 1.57f; 
		mNode->translate(SinbadTranslate * 1 * _WalkingSpeed); 
		mNode->resetOrientation(); //
		mNode->yaw(Ogre::Radian(_rotation));
		walked = true; 
	}
	if (mKeyboard->isKeyDown(OIS::KC_Z))
	{
		_aniStateDance->setEnabled(true);
		if (_aniStateDance->hasEnded())
		{
			_aniStateDance->setTimePosition(0.0f);
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_X))
	{
		_aniStateSH->setEnabled(true);
		if (_aniStateSH->hasEnded())
		{
			_aniStateSH->setTimePosition(0.0f);
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_C))
	{
		_aniStateSV->setEnabled(true);
		if (_aniStateSV->hasEnded())
		{
			_aniStateSV->setTimePosition(0.0f);
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_V))
	{
		_aniStateIB->setEnabled(true);
		//_aniStateIT->setEnabled(true);
		if (_aniStateIB->hasEnded())
		{
			_aniStateIB->setTimePosition(0.0f);
		}
	}
	if(walked) 
	{
		_aniState->setEnabled(true); 
		_aniStateTop->setEnabled(true); 
		if(_aniState->hasEnded())
		{
			_aniState->setTimePosition(0.0f);
		} 
		if(_aniStateTop->hasEnded()) 
		{
			_aniStateTop->setTimePosition(0.0f); 
		}
	}

	if (mKeyboard->isKeyDown(OIS::KC_R))
	{
		if (mDirection == Vector3::ZERO) 
		{
			if (nextLocation()) 
			{
				mAnimationState = mEntity->getAnimationState("RunBase"); //Walk
				mAnimationState->setLoop(true); 
				mAnimationState->setEnabled(true);
			} 
		}
		else 
		{
			Real move = mWalkSpeed * 1;//evt.timeSinceLastFrame; 
			mDistance -= move;


			if (mDistance <= 0.0f) 
			{
				mNode->setPosition(mDestination); 
				mDirection = Vector3::ZERO;
				if (! nextLocation()) 
				{
					mAnimationState = mEntity->getAnimationState("IdleBase");//Idle
					mAnimationState->setLoop(true); 
					mAnimationState->setEnabled(true);
				}
				else 
				{
					// Rotation Code will go here later 
				}
			}
			else 
			{
				mNode->translate(mDirection * move); 
			} // else 
		}
		mAnimationState->addTime(1);//evt.timeSinceLastFrame);
	}
	if (mKeyboard->isKeyDown(OIS::KC_L))
	{
		light=mSceneMgr->getLight("Light1");
		if (light->getVisible()==false)
		{
			light->setVisible(true);
		}
		else
		{
			light->setVisible(false);
		}
	}
	if(e.key ==OIS::KC_TAB){
		mRobotMode =!mRobotMode;
	}
	return true;
}

bool CMyFrameListener::keyReleased(const OIS::KeyEvent &e)
{
	 mCameraMan->injectKeyUp(e);
	return true;
}

bool CMyFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{	
	//Ogre::Vector3 SinbadTranslate(0,0,0);
	mKeyboard->capture();
	mMouse->capture();
	mTrayMgr->refreshCursor();
	mTrayMgr->frameRenderingQueued(evt);
	if (!FrameListener::frameStarted(evt)) 
	return false;
	Vector3 camPos = mCamera->getPosition(); 
	Vector3 nodePos = mNode->getPosition();
	Ray cameraRay(Vector3(camPos.x, 5000.0f, camPos.z), Vector3::NEGATIVE_UNIT_Y); 
	Ray cameraRay1(Vector3(nodePos.x, 5000.0f, nodePos.z), Vector3::NEGATIVE_UNIT_Y); 
	mRaySceneQuery->setRay(cameraRay);
	mRaySceneQuery->setRay(cameraRay1);
	RaySceneQueryResult &result = mRaySceneQuery->execute(); 
	RaySceneQueryResult::iterator itr = result.begin();
	if (itr != result.end() && itr->worldFragment) {
	Real terrainHeight = itr->worldFragment->singleIntersection.y; 
	mNode->setPosition( nodePos.x, terrainHeight + 50.0f , nodePos.z );
	if ((terrainHeight + 10.0f) > camPos.y) 
	mCamera->setPosition( camPos.x, terrainHeight + 20.0f, camPos.z );
	}

	float rotX= mMouse->getMouseState().X.rel* evt.timeSinceLastFrame*-0.5;
	float rotY=mMouse->getMouseState().Y.rel*evt.timeSinceLastFrame* -0.5;
	mCamera->yaw(Ogre::Radian(rotX));
	mCamera->pitch(Ogre::Radian(rotY));
	return true;
}

