#include"ball.h"
#include"cocos2d.h"
Scene* PhysicsTest::createScene()
{
	Scene* scene = NULL;
	do
	{
		scene = Scene::createWithPhysics();
		//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		CC_BREAK_IF(!scene);
		scene->getPhysicsWorld()->setGravity (Vect(0.0f, -9.87f));
		auto layer = PhysicsTest::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
		return scene;
	} while (0);
	return scene;
}
bool PhysicsTest::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto edgeSp = Sprite::create();//创建一个精灵
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeSp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	edgeSp->setPhysicsBody(body);
	this->addChild(edgeSp);

	std::vector<std::string>imageNames;
	imageNames.push_back("basketball.png");
	imageNames.push_back("football.png");

	auto planeListener = EventListenerTouchOneByOne::create();
	planeListener->onTouchBegan = [](Touch* touch, Event* event){return true; };
	planeListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		Vec2 touchLocation = touch->getLocation();
		//Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
		srand((unsigned int)time(0)); //设置随机种子
		int rand = random(0, 1);
		//int rand = arc4random() % 2;
		std::string imageName = imageNames.at(rand);
		Sprite* ball = Sprite::create(imageName);
		//ball->setPosition(nodeLocation.x, nodeLocation.y);
		ball->setPosition(touchLocation.x,touchLocation.y);
		auto body = PhysicsBody::createCircle(ball->getContentSize().width / 2);
		body->setVelocity(Vect(0.0f, -100.0f));//设置初始速度
		PhysicsShape* ps = body->getShape(0);
		ps->setMoment(0.2f);
		ps->setFriction(0.2f);
		ps->setRestitution(1.0f);
		ball->setPhysicsBody(body);
		this->addChild(ball);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(planeListener, this);
	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(PhysicsTest::onAcceleration, this));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
void PhysicsTest::onAcceleration(Acceleration* acc, Event* event)
{
	Vect gravity(-acc->y * 50, acc->x * 50);
	this->getPhysicsBody()->getWorld()->setGravity(gravity);
}