#include "TerrainTest.h"

bool Player::isDone() const
{
    return false;
}


void Player::update(float dt)
{
    auto player = (Sprite3D *)this;
    switch (_playerState)
    {
    case PLAYER_STATE_IDLE:
        break;
    case PLAYER_STATE_FORWARD:
        {
            Vec3 curPos=  player->getPosition3D();
            Vec3 newFaceDir = _targetPos - curPos;
            newFaceDir.y = 0.0f;
            newFaceDir.normalize();
            Vec3 offset = newFaceDir * 25.0f * dt;
            curPos+=offset;
            player->setPosition3D(curPos);
        }
        break;
    case PLAYER_STATE_BACKWARD:
        {
            Vec3 forward_vec;
            player->getNodeToWorldTransform().getForwardVector(&forward_vec);
            forward_vec.normalize();
            player->setPosition3D(player->getPosition3D()-forward_vec*15*dt);
        }
        break;
    case PLAYER_STATE_LEFT:
        {
            player->setRotation3D(player->getRotation3D()+Vec3(0,25*dt,0));
        }
        break;
    case PLAYER_STATE_RIGHT:
        {
            player->setRotation3D(player->getRotation3D()+Vec3(0,-25*dt,0));
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
    float player_h = _terrain->getHeight(playerPos.x, playerPos.z,&Normal);
    if (Normal.isZero())//check the player whether is out of the terrain
    {
        player_h = playerPos.y;
    }
    else
    {
        player_h += PLAYER_HEIGHT;
    }
    player->setPositionY(player_h);
    Quaternion q2;
    q2.createFromAxisAngle(Vec3(0,1,0),(float)-M_PI,&q2);
 
    Quaternion headingQ;
    headingQ.createFromAxisAngle(_headingAxis,_headingAngle,&headingQ);
    player->setRotationQuat(headingQ*q2);
    auto vec_offset =Vec4(camera_offset.x,camera_offset.y,camera_offset.z,1);
    vec_offset = player->getNodeToWorldTransform()*vec_offset;
     _cam->setPosition3D(player->getPosition3D() + camera_offset);
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
    _playerState = PLAYER_STATE_IDLE;
}


void Player::forward()
{
    _playerState = PLAYER_STATE_FORWARD;
}

void Player::backward()
{
    _playerState = PLAYER_STATE_BACKWARD;
}

void Player::updateState()
{
    auto player = (Sprite3D * )this;
    switch (_playerState)
    {
    case PLAYER_STATE_FORWARD:
        {
            Vec2 player_pos =Vec2(player->getPositionX(),player->getPositionZ());
            Vec2 targetPos = Vec2(_targetPos.x,_targetPos.z);
            auto dist = player_pos.distance(targetPos);
            if(dist<1)
            {
                _playerState = PLAYER_STATE_IDLE;
            }
        }
        break;
    default:
        break;
    }
}

Player * Player::create(const char * file,Camera * cam,Terrain * terrain)
{
    //
    auto sprite = new (std::nothrow) Player();
    if (sprite && sprite->initWithFile(file))
    {
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

