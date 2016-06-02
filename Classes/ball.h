#ifndef _BALL_H_
#define _BALL_H_
#include"cocos2d.h"
USING_NS_CC;
class PhysicsTest:public Layer
{
public:
	static Scene* createScene();
	bool init();
	CREATE_FUNC(PhysicsTest);
	void onAcceleration(Acceleration* acc,Event* event);
};
#endif