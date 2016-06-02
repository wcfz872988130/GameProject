#ifndef _PLAYER_H_
#define  _PLAYER_H_
#include"cocos2d.h"
#include "3d/CCBundle3D.h"
#include "physics3d/CCPhysics3D.h"
#include"physics3d/CCPhysics3D.h"

//角色的各种状态
#define MODEL_PATH "Sprite3DTest/rat.c3b"
#define PLAYER_STATE_LEFT 0 
#define PLAYER_STATE_RIGHT 1
#define PLAYER_STATE_IDLE 2
#define PLAYER_STATE_FORWARD 3
#define PLAYER_STATE_BACKWARD 4
#define PLAYER_STATE_BACK 5

#define IDLE_STRAT 0
#define IDLE_END 23//idle 0~23f
#define KONCKED_STRAT 30
#define KONCKED_END 37//koncked 30~37f
#define DEAD_STRAT 41
#define DEAD_END 76//dead 41~76f
#define ATTACK_START 81
#define ATTACK_END 117//attack 81~117f
#define FORWARD_START 122
#define FORWARD_END 142//forward 122~142f
#define FRAME_RATE 30//动画播放的帧率

#define  FRAME_RATE 30
#define CAMERA_DISTANCE 80
#define FORWARD_SPEED 20.0f
USING_NS_CC;
class Player :public cocos2d::PhysicsSprite3D //public cocos2d::Sprite3D 
{
public:
	static Player* create(const char* file, Camera* cam, Terrain* terrain, Physics3DColliderDes* des);
	bool isDone() const;
	void update(float dt);
	void rotCamera();
	Player();
	~Player();
	void turnLeft();
	void forward();
	void turnRight();
	void backward();
	void idle();
	void back();
	Vec3 _targetPos;
	void updateState();
	float _headingAngle;
	Vec3 _headingAxis;
private:
	Terrain* _terrain;
	Camera* _cam;
	int _playerState;
	bool isFinish;
	Vec3 _cameraOffset;
};
#endif