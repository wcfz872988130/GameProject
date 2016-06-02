#include"player.h"
#include "cocos2d.h"
#include "math.h"

USING_NS_CC;
#define PLAYER_HEIGHT 0
Vec3 camera_offset(0, 80, 80);

Player::Player()
{
	isFinish = true;
}
Player::~Player()
{
}

bool Player::isDone() const
{
	return false;
}

void Player::update(float dt)
{
	auto player = (Sprite3D*)this;
	//_cam->setPosition3D(player->getPosition3D() + camera_offset);
	switch (_playerState)
	{
	case PLAYER_STATE_IDLE:
	{
	}
	break;
	case PLAYER_STATE_FORWARD:
	{
		Vec3 curPos = player->getPosition3D();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();//使向量的长度为单位长度
		Vec3 offset = newFaceDir*25.0f*dt;
		curPos += offset;
		player->setPosition3D(curPos);
		_cam->setPosition3D(player->getPosition3D() + camera_offset);
	}
	break;
	case PLAYER_STATE_BACKWARD:
	{
		//Vec3 forward_vec;
		//player->getNodeToWorldTransform().(&forward_vec);
		//forward_vec.normalize();
		//player->setPosition3D(player->getPosition3D()-forward_vec*15*dt);
	}
	break;
	case PLAYER_STATE_LEFT:
	{
		player->setRotation3D(player->getRotation3D()+Vec3(0,25*dt,0));
	}
	break;
	case PLAYER_STATE_RIGHT:
	{
		player->setRotation3D(player->getRotation3D() + Vec3(0,-25*dt,0));
	}
	break;
	case PLAYER_STATE_BACK:
	{
		Vec3 curPos = player->getPosition3D();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();//使向量的长度为单位长度
		Vec3 offset = newFaceDir*(-5.0f) * 1;
		curPos += offset;
		_targetPos = curPos;
		player->setPosition3D(curPos);
	}
	break;
	default:
		break;
	}
	// transform player position to world coord
	auto playerPos = player->getPosition3D();
	auto playerModelMat = player->getParent()->getNodeToWorldTransform();
	playerModelMat.transformPoint(&playerPos);
	Vec3 Normal;
	float player_h = _terrain->getHeight(playerPos.x, playerPos.z, &Normal);
	if (Normal.isZero())//check the player whether is out of the terrain//如果向量所有的分量都为0则返回true，否则返回false
	{
		player_h = playerPos.y;
	}
	else
	{
		player_h += PLAYER_HEIGHT;
	}
	player->setPositionY(player_h);
	Quaternion q2;
	q2.createFromAxisAngle(Vec3(0, 1, 0), (float)-M_PI, &q2);//从旋转轴及旋转角度生成一个四元数，Param1:旋转轴,Param2:旋转角度（弧度）,Param3:存储的四元数

	Quaternion headingQ;
	headingQ.createFromAxisAngle(_headingAxis, _headingAngle, &headingQ);
	player->setRotationQuat(headingQ*q2);//先旋转headingQ，再旋转q2；
	auto vec_offset = Vec4(camera_offset.x, camera_offset.y, camera_offset.z, 1);
	vec_offset = player->getNodeToWorldTransform()*vec_offset;
	//_cam->setPosition3D(player->getPosition3D() + camera_offset);
	updateState();
}

void Player::turnLeft()
{
	_playerState = PLAYER_STATE_LEFT;
}

void Player::turnRight()
{
	_playerState = PLAYER_STATE_RIGHT;
}

void Player::idle()
{
	if (_playerState != PLAYER_STATE_IDLE)
	{
		_playerState = PLAYER_STATE_IDLE;
		auto animation = Animation3D::create(MODEL_PATH);
		if (animation)
		{
			auto animate = Animate3D::createWithFrames(animation, IDLE_STRAT, IDLE_END, FRAME_RATE);
			this->stopAllActions();
			this->runAction(RepeatForever::create(animate));
		}
	}
}


void Player::forward()
{
	if (_playerState != PLAYER_STATE_FORWARD)//如果不处于前进状态，则切换为前进状态，防止重复播放动画
	{
		_playerState = PLAYER_STATE_FORWARD;
		auto animation = Animation3D::create(MODEL_PATH);
		if (animation)
		{
			auto animate = Animate3D::createWithFrames(animation, FORWARD_START, FORWARD_END, FRAME_RATE);
			this->stopAllActions();
			this->runAction(RepeatForever::create(animate));
		}
	}
}

void Player::backward()
{
	_playerState = PLAYER_STATE_BACKWARD;
}

void Player::back()
{
	_playerState = PLAYER_STATE_BACK;
}

void Player::updateState()
{
	auto player = (Sprite3D *)this;
	switch (_playerState)
	{
	case PLAYER_STATE_FORWARD:
	{
		Vec2 player_pos = Vec2(player->getPositionX(), player->getPositionZ());
		Vec2 targetPos = Vec2(_targetPos.x, _targetPos.z);
		auto dist = player_pos.distance(targetPos);
		if (dist < 1)
		{
			this->idle();
		}
	    if (player->getPositionX() <= -434 || player->getPositionX() >= 434 || player->getPositionZ() <= -434)
		{
			this->back();
		}
		else if (player->getPositionZ() >= 434)
		{
			this->back();
			if (isFinish==true)
			{
				isFinish = false;
				//this->rotCamera();			
			}			
		}

		/*Vec3 Normal;
		float Location_h = _terrain->getHeight(player->getPositionX(), player->getPositionZ(), &Normal);
		float _terrainAxis;
		_terrainAxis = acos(Normal.dot(this->_headingAxis));
		if (_terrainAxis >= (M_PI / 4) || _terrainAxis <= -(M_PI / 4))
		{
			this->back();
		}*/
	}
		break;
	default:
		break;
	}
}

Player*  Player::create(const char * file, Camera * cam, Terrain * terrain, Physics3DColliderDes* des)
{
	//
	auto sprite = new (std::nothrow) Player();//在内存不足时,new(std::nothrow)不会抛出异常，而是将指针置为零
	if (sprite && sprite->initWithFile(file))
	{
		auto obj = Physics3DCollider::create(des);
		sprite->_physicsComponent = Physics3DComponent::create(obj);
		sprite->addComponent(sprite->_physicsComponent);
		sprite->_contentSize = sprite->getBoundingBox().size;
		//sprite->_cameraOffset = Vec3(0,CAMERA_DISTANCE,CAMERA_DISTANCE);
		sprite->_headingAngle = 0;
		sprite->_playerState = PLAYER_STATE_IDLE;
		sprite->_cam = cam;
		sprite->_terrain = terrain;
		sprite->autorelease();
		sprite->scheduleUpdate();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Player::rotCamera()
{
	auto player = (Sprite3D *)this;
	float toBoundaryDistance = 80 * 80 + 80 * 80;
	//_cam->setPosition3D(player->getPosition3D()+Vec3(0,80,-80));
	//_cam->lookAt(player->getPosition3D());
	for (int i = 0; i <= 45; i++)
	{
		float camera_z = std::sqrt(toBoundaryDistance)*std::cos((45 + i)*M_PI/180);
		float camera_y = std::sqrt(toBoundaryDistance)*std::sin((45 + i)*M_PI/180);
		Vec3 camera_offset_T(0, camera_y, camera_z);
		_cam->setRotation3D(Vec3(-i, 0, 0));
		for (int j = 0; j < 100000; j++)
		{
		_cam->setPosition3D(player->getPosition3D() + camera_offset_T);
		
		//_cam->lookAt(player->getPosition3D());		
		}
	}
	for (int i = 0; i <= 45; i++)
	{
		float camera_z = std::sqrt(toBoundaryDistance)*std::sin(i*M_PI/180);
		float camera_y = std::sqrt(toBoundaryDistance)*std::cos(i*M_PI/180);
		Vec3 camera_offset_T(0, camera_y, camera_z);
		_cam->setRotation3D(Vec3(-i, 0, 0));
		for (int j = 0; j < 100000; j++)
		{
		_cam->setPosition3D(player->getPosition3D() + camera_offset_T);
		//_cam->lookAt(player->getPosition3D());
			
		}
	}
	//_cam->lookAt(player->getPosition3D());
	isFinish = true;
}