#ifndef _LIGHTDEMO_H_
#define _LIGHTDEMO_H_
#include"cocos2d.h"
#include"player.h"
#include "3d/CCBundle3D.h"
#include "physics3d/CCPhysics3D.h"
#include"Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;


#define START_POS_X -0.5
#define START_POS_Y -2.5
#define START_POS_Z -0.5

#define ARRAY_SIZE_X 4
#define ARRAY_SIZE_Y 3
#define ARRAY_SIZE_Z 4

#define MODEL_PATH "Sprite3DTest/rat.c3b"

class LightDemo :public Scene
{
public:
	static LightDemo* createScene();
	bool init();
	CREATE_FUNC(LightDemo);
	void initUI();
	void onButtonClick(Ref* sender);
	bool onTouchesBegan(Touch* touch, Event* event);
	void onTouchesEnded(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch,Event* event);
	void ensureWalk(float dt);
private:
	float _headingAngle;
	float _terrainAxis;
	Layer* _layer3D;
	Terrain* _terrain;
	Terrain* _terrain_2;
	BillBoard* _arrow;
	Player* _player;
	Sprite3D* _struct;
	Sprite3D* _Bigtree[17];
	Sprite3D* _Bigtree_2[17];
	Sprite3D* _Bigtree_3[17];
	Sprite3D* _Bigtree_4[17];
	Camera* _camera;
	Skybox* _skyBox;
	TextureCube* _textureCube;
	AmbientLight* _ambientLight;
	DirectionLight* _directionLight;
	PointLight* _pointLight;
	SpotLight* _spotLight;
	BillBoard* _billBoard;
	BillBoard* _tree[20];
	BillBoard* _snow[50];
	Vec3 collisionPoint;
	PhysicsSprite3D* _orc;
};
#endif