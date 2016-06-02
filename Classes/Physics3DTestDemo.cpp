#include"Physics3DTestDemo.h"
#include"3d/CCTerrain.h"
#include"3d/CCBundle3D.h"
#include"physics3d/CCPhysics3D.h"
#include"extensions/Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC_EXT;

static cocos2d::Scene* physicsScene = nullptr;

Physics3DTestDemo::Physics3DTestDemo() :_angle(0), _camera(nullptr)
{

}

Physics3DTestDemo::~Physics3DTestDemo()
{

}

bool Physics3DTestDemo::init()
{
	if (!TestCase::init())return false;

	if (initWithPhysics())
	{
		getPhysics3DWorld()->setDebugDrawEnable(false);
		physicsScene = this;
		Size size = Director::getInstance()->getWinSize();
		_camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 1000.0f);
		_camera->setPosition3D(Vec3(0.0f,50.0f,100.0f));
		_camera->lookAt(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,1.0f,0.0f));
		_camera->setCameraFlag(CameraFlag::USER1);
		this->addChild(_camera);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(Physics3DTestDemo::touchbegan,this);
		listener->onTouchMoved = CC_CALLBACK_2(Physics3DTestDemo::touchmoved,this);
		listener->onTouchEnded = CC_CALLBACK_2(Physics3DTestDemo::touchend,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_angle = 0.0f;
		return true;
	}
	physicsScene = nullptr;
	return false;
}

std::string Physics3DTestDemo::title() const
{
	return "Physics3D Test";
}

std::string Physics3DTestDemo::subtitle() const
{
	return "";
}

void Physics3DTestDemo::update(float delta)
{
}

bool  Physics3DTestDemo::touchbegan(Touch* touch, Event* event)
{
	return true;
}

void Physics3DTestDemo::touchmoved(Touch* touch, Event* event)
{
}

void Physics3DTestDemo::touchend(Touch* touch, Event* event)
{
}



