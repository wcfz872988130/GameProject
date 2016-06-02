#ifndef _COLLISION_H_
#define _COLLISION_H_
#include <string>
#include"Physics3DTestDemo.h"
#include "cocos2d.h"
USING_NS_CC;
class Physics3DTerrainDemo :public Scene
{
public:
	CREATE_FUNC(Physics3DTerrainDemo);
	static Scene* createScene();
	bool init();
	Physics3DTerrainDemo(){};
	~Physics3DTerrainDemo(){};
	std::string subtitle()const;
private:
	Camera* _camera;
	Layer* physicsScene;
};
#endif