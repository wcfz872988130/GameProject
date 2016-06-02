#ifndef _PHYSICS3DTESTDEMO_H_
#define _PHYSICS3DTESTDEMO_H_
#include "cocos2d.h"
#include "BaseTest.h"
USING_NS_CC;
class Physics3DTestDemo:public TestCase
{
public:
	CREATE_FUNC(Physics3DTestDemo);
	Physics3DTestDemo();
	~Physics3DTestDemo();

	virtual bool init() override;
	virtual std::string title() const override;
	virtual std::string subtitle() const override;
	virtual void update(float delta) override;

    bool touchbegan(Touch* touch,Event* event);
	void touchmoved(Touch* touch,Event* event);
	void touchend(Touch* touch,Event* event);
protected:
	std::string _title;
	cocos2d::Camera* _camera;
	float _angle;
};
#endif