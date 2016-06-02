#include "LightDemo.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "3d/CCBundle3D.h"
#include "physics3d/CCPhysics3D.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
using namespace ui;
using namespace cocostudio;

LightDemo* LightDemo::createScene()
{
	/*Scene* scene = NULL;
	do
	{
		scene = Scene::create();
		CC_BREAK_IF(!scene);
		auto layer = LightDemo::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);
	return scene;*/
	LightDemo* pRet = new(std::nothrow)LightDemo();
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool LightDemo::init()
{
	bool Bref = false;
	do
	{
		if (initWithPhysics())
		{

			getPhysics3DWorld()->setDebugDrawEnable(true);
			//_terrain->setDrawWire(true);
			auto S = Director::getInstance()->getVisibleSize();
			_layer3D = Layer::create();
			//_layer3D->ignoreAnchorPointForPosition(false);
			//_layer3D->setAnchorPoint(Point(0.5f, 0.5f));
			this->addChild(_layer3D);
			/*设置相机*/
			auto pCamera = Camera::createPerspective(60, (GLfloat)S.width / S.height, 0.1, 300);
			pCamera->setDepth(100);
			_camera = pCamera;
			//pCamera->setPosition3D(Vec3(-1, 1.6, 4));
			//pCamera->lookAt(Vec3(50, 0, 50));
			pCamera->setCameraFlag(CameraFlag::USER1);
			_layer3D->addChild(_camera);
			//_layer3D->setPhysics3DDebugCamera(_camera);

			/*创建地形*/
			Terrain::DetailMap r("terrain/dirt.jpg");
			Terrain::DetailMap g("terrain/Grass2.jpg", 5);
			Terrain::DetailMap b("terrain/road.jpg");
			Terrain::DetailMap a("terrain/GreenSkin.jpg", 10);
			//Terrain::TerrainData data("terrain/heightmap16.jpg", "terrain/alphamap.png", r, g, b, a, Size(32, 32), 40.0f, 2);
			Terrain::TerrainData data("terrain/test.png", "terrain/ground.png", r, g, b, a, Size(32, 32), 0.0f, 5);
			//Terrain::TerrainData data("terrain/heightmap16.jpg", "terrain/alphamap.png", r, g, b, a);
			_terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
			_terrain->setMaxDetailMapAmount(4);
			_terrain->setDrawWire(false);
			_terrain->setSkirtHeightRatio(3);
			//_terrain->setLODDistance(3.2, 6.4, 9.6);
			_terrain->setLODDistance(64, 128, 192);
			_terrain->setCameraMask(2);
			_layer3D->addChild(_terrain);

			/*导入物体模型*/
			std::string fileName1 = "model/tree.c3b";
			_struct = Sprite3D::create(fileName1);
			_struct->setCameraMask(2);
			_struct->setLightMask(0);
			_struct->setScale(0.08f);
			_struct->setPositionY(_terrain->getHeight(_struct->getPositionX(), _struct->getPositionZ()));
			_layer3D->addChild(_struct);

			for (int i = 0; i <= 16; i++)
			{
				_Bigtree[i] = Sprite3D::create(fileName1);
				_Bigtree[i]->setCameraMask(2);
				_Bigtree[i]->setLightMask(0);
				_Bigtree[i]->setScale(0.05f);
				_Bigtree[i]->setPosition3D(Vec3(-434 + i * 50, 0, -434));
				_Bigtree[i]->setPositionY(_terrain->getHeight(_Bigtree[i]->getPositionX(), _Bigtree[i]->getPositionZ()));
				_layer3D->addChild(_Bigtree[i]);
			}

			for (int i = 0; i <= 16; i++)
			{
				_Bigtree_2[i] = Sprite3D::create(fileName1);
				_Bigtree_2[i]->setCameraMask(2);
				_Bigtree_2[i]->setLightMask(0);
				_Bigtree_2[i]->setScale(0.05f);
				_Bigtree_2[i]->setPosition3D(Vec3(-434 + i * 50, 0, 434));
				_Bigtree_2[i]->setPositionY(_terrain->getHeight(_Bigtree_2[i]->getPositionX(), _Bigtree_2[i]->getPositionZ()));
				_layer3D->addChild(_Bigtree_2[i]);
			}

			for (int i = 0; i <= 16; i++)
			{
				_Bigtree_3[i] = Sprite3D::create(fileName1);
				_Bigtree_3[i]->setCameraMask(2);
				_Bigtree_3[i]->setLightMask(0);
				_Bigtree_3[i]->setScale(0.05f);
				_Bigtree_3[i]->setPosition3D(Vec3(-434, 0, -434 + (i + 1) * 50));
				_Bigtree_3[i]->setPositionY(_terrain->getHeight(_Bigtree_3[i]->getPositionX(), _Bigtree_3[i]->getPositionZ()));
				_layer3D->addChild(_Bigtree_3[i]);
			}

			for (int i = 0; i <= 16; i++)
			{
				_Bigtree_4[i] = Sprite3D::create(fileName1);
				_Bigtree_4[i]->setCameraMask(2);
				_Bigtree_4[i]->setLightMask(0);
				_Bigtree_4[i]->setScale(0.05f);
				_Bigtree_4[i]->setPosition3D(Vec3(434, 0, -434 + (i + 1) * 50));
				_Bigtree_4[i]->setPositionY(_terrain->getHeight(_Bigtree_4[i]->getPositionX(), _Bigtree_4[i]->getPositionZ()));
				_layer3D->addChild(_Bigtree_4[i]);
			}

			/*导入场景*/
			auto rootNode = CSLoader::createNode("Scene3D.csb");
			_layer3D->addChild(rootNode);

			// 获取摄像机
			//auto s_camera = (Camera*)rootNode->getChildByName("UserCamera_1");
			//s_camera->setCameraFlag(CameraFlag::USER1);
			//s_camera->setDepth(1000);

			// 获取场景模型
			auto _sprite3D = static_cast<Sprite3D*>(rootNode->getChildByName("Aurora"));
			_sprite3D->setPositionY(_terrain->getHeight(_sprite3D->getPositionX(), _sprite3D->getPositionZ()) - 4);
			_sprite3D->setCameraMask(2);
			_sprite3D->setLightMask(0);
			_sprite3D->setScale(0.03);

			/*	std::vector<float>heidata = _terrain->getHeightData();
			auto size = _terrain->getTerrainSize();
			Physics3DColliderDes colliderDes;
			colliderDes.shape = Physics3DShape::createHeightfield(size.width,size.height,&heidata[0],1.0f,_terrain->getMinHeight(),_terrain->getMaxHeight(),true,false,true);
			auto collider = Physics3DCollider::create(&colliderDes);
			auto component = Physics3DComponent::create(collider);
			_terrain->addComponent(component);
			_layer3D->addChild(_terrain);
			component->syncNodeToPhysics();
			component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);*/

			auto shader = GLProgram::createWithFilenames("Sprite3DTest/cube_map.vert", "Sprite3DTest/cube_map.frag");
			auto state = GLProgramState::create(shader);
			_textureCube = TextureCube::create("Sprite3DTest/skybox/left.jpg",
				"Sprite3DTest/skybox/right.jpg",
				"Sprite3DTest/skybox/top.jpg",
				"Sprite3DTest/skybox/bottom.jpg",
				"Sprite3DTest/skybox/front.jpg",
				"Sprite3DTest/skybox/back.jpg");

			/*设置纹理参数*/
			Texture2D::TexParams tRepeatParams;
			tRepeatParams.magFilter = GL_LINEAR;
			tRepeatParams.minFilter = GL_LINEAR;
			tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
			tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
			_textureCube->setTexParameters(tRepeatParams);
			state->setUniformTexture("u_cubeTex", _textureCube);

			/*创建天空盒*/
			_skyBox = Skybox::create();
			//_skyBox->setCameraMask(s_CM[LAYER_BACKGROUND]);
			_skyBox->setCameraMask(2);
			_skyBox->setTexture(_textureCube);
			_skyBox->setScale(1200.f);
			_skyBox->setGlobalZOrder(-1);
			_layer3D->addChild(_skyBox);

			/*添加玩家*/
			/*std::string fileName = "Sprite3DTest/girl.c3b";
			_player = Player::create("Sprite3DTest/girl.c3b", _camera, _terrain);
			_player->setCameraMask(2);
			//_player->syncNodeToPhysics();
			//_player->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
			_player->setLightMask(0);
			_player->setScale(0.08f);
			_player->setPositionY(_terrain->getHeight(_player->getPositionX(), _player->getPositionZ()));
			auto animation = Animation3D::create(fileName);
			if (animation)
			{
			auto animate = Animate3D::create(animation);
			_player->runAction(animate);
			}
			_layer3D->addChild(_player);*/
			std::string fileName = MODEL_PATH;
			
			

			/*添加怪物*/
			auto orcParam = Sprite3D::create("Sprite3DTest/orc.c3b");
			Vec3 maxP = orcParam->getAABB()._max;
			Vec3 minP = orcParam->getAABB()._min;
			int height = abs(maxP.y - minP.y);
			int length = abs(maxP.x - minP.x);
			int width = abs(maxP.z - minP.z);
			int radius;
			if (length >= width)
			{
				radius = length;
			}
			else
				radius = width;

			Physics3DColliderDes playerDes;
			playerDes.shape = Physics3DShape::createCapsule(0.2*radius, 0.07*height);
			playerDes.isTrigger = true;
			_player = Player::create(MODEL_PATH, _camera, _terrain, &playerDes);
			_player->syncNodeToPhysics();
			_player->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NODE_AND_NODE);
			_player->getPhysicsObj()->setUserData(_player);

			Physics3DColliderDes orcDes;
			orcDes.shape = Physics3DShape::createCapsule(0.4*radius, 0.07*height);
			_orc = PhysicsSprite3D::createWithCollider("Sprite3DTest/orc.c3b", &orcDes, Vec3(0, -4 * 0.07*height, 0));
			_orc->syncNodeToPhysics();
			_orc->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NODE_AND_NODE);
			_orc->getPhysicsObj()->setUserData(_orc);
			_orc->setCameraMask((unsigned short)CameraFlag::USER1);
			_orc->setLightMask(0);
			_orc->setScale(1.0f);
			_orc->setPosition3D(_player->getPosition3D() + Vec3(50, 0, 70));
			_orc->setPositionY(_terrain->getHeight(_orc->getPositionX(), _orc->getPositionZ()));
			_orc->setRotation3D(Vec3(0, 180, 0));
			_layer3D->addChild(_orc);

			auto slime = Sprite3D::create("Sprite3DTest/orc.c3b");
			slime->setCameraMask((unsigned short)CameraFlag::USER1);
			slime->setLightMask(0);
			slime->setScale(20.0f);
			slime->setPosition3D(_player->getPosition3D() + Vec3(50, 30, 50));
			slime->setPositionY(_terrain->getHeight(slime->getPositionX(), slime->getPositionZ()));
			//slime->setRotation3D(Vec3(0, 180, 0));
			_layer3D->addChild(slime);

			_player->getPhysicsObj()->setCollisionCallback([=](const Physics3DCollisionInfo &ci)
			{
				if (!ci.collisionPointList.empty())
				{
					if (ci.objB->getUserData()==_player&&ci.objA->getUserData()==_orc)
					{
						Vec3 curPos = _player->getPosition3D();
						Vec3 newFaceDir = curPos - _player->_targetPos;
						newFaceDir.y = 0.0f;
						newFaceDir.normalize();
						Vec3 offset = newFaceDir*0.10*FORWARD_SPEED*0.167;
						curPos += offset;				
						_player->setPosition3D(curPos);
						_player->idle();
					}
				}
			});
			_player->setCameraMask(2);
			_player->setLightMask(0);
			_player->setScale(1.0f);
			//_player->setPosition3D(Vec3(-514,0,-514));
			_player->setPositionY(_terrain->getHeight(_player->getPositionX(), _player->getPositionZ()));
			_layer3D->addChild(_player);
			
			//_camera->setRotation3D(Vec3(-45, 0, 0));
			_camera->setPosition3D(_player->getPosition3D() + Vec3(0, 80, 80));
			//_camera->setPosition3D(Vec3(0, 10080, 0));
			_camera->lookAt(_player->getPosition3D());
			//_camera->setRotation3D(Vec3(-45, 0, 0));
			//_camera->setRotation3D(Vec3(-60, 0, 0));

			_layer3D->setPosition3D(Vec3(0, 10000, 0));

			/*灯光控制*/
			/*环境光*/
			_ambientLight = AmbientLight::create(Color3B(0, 255, 0));
			_ambientLight->setEnabled(false);
			_ambientLight->setCameraMask(2);
			_layer3D->addChild(_ambientLight);

			/*方向光*/
			_directionLight = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B(0, 255, 0));
			_directionLight->setCameraMask(2);
			_directionLight->setEnabled(false);
			_layer3D->addChild(_directionLight);

			/*点光源*/
			_pointLight = PointLight::create(Vec3(0.0f, 50.0f, 0.0f), Color3B(0, 255, 0), 500.0f);
			_pointLight->setEnabled(false);
			_pointLight->setCameraMask(2);
			_layer3D->addChild(_pointLight);

			/*聚光灯*/
			_spotLight = SpotLight::create(Vec3(0.f, -1.0f, 0.0f), Vec3(0.0f, 50.0f, 0.0f), Color3B(0, 255, 0), 0.0, 0.5, 10000.0f);
			_spotLight->setCameraMask(2);
			_spotLight->setEnabled(false);
			_layer3D->addChild(_spotLight);

			/*增加3D粒子*/
			auto rootps = PUParticleSystem3D::create("Particle3D/scripts/mp_torch.pu");
			rootps->setCameraMask((unsigned short)CameraFlag::USER1);
			rootps->setScale(2.0f);
			rootps->startParticleSystem();
			rootps->setPosition3D(_player->getPosition3D() + Vec3(0, 1, 0));
			_player->addChild(rootps);

			/*增加广告牌*/
			_billBoard = BillBoard::create("blood.png");
			_billBoard->setScale(0.05f);
			//_billBoard->setAnchorPoint(Vec2(0, 0.5));
			_billBoard->setPosition3D(_player->getPosition3D() + Vec3(0, 18, 0));
			_billBoard->setCameraMask((unsigned short)CameraFlag::USER1);
			_player->addChild(_billBoard);

			/*增加树*/
			for (int i = 0; i < 20; i++)
			{
				if (i % 2)
				{
					_tree[i] = BillBoard::create("tree.png");
				}
				else
				{
					_tree[i] = BillBoard::create("tree2.png");
				}
				_tree[i]->setScale(rand_0_1()*0.01 + 0.05);
				_tree[i]->setAnchorPoint(Vec2(0, 0));
				auto locationx = rand_0_1() * 200;
				auto locationz = rand_0_1() * 200;
				auto locationy = _terrain->getHeight(locationx, locationz);
				_tree[i]->setPosition3D(Vec3(locationx, locationy, locationz));
				//+_tree[i]->getContentSize().height / 2
				_tree[i]->setCameraMask(2);
				_tree[i]->setVisible(true);
				_layer3D->addChild(_tree[i]);
			}

			/*添加平面建筑*/
			auto building = Sprite::create("building1.png");
			building->setPosition3D(_player->getPosition3D() + Vec3(-430,0,60));
			building->setScale(0.1);
			building->setCameraMask(2);
			_layer3D->addChild(building);

			/*增加点击事件*/
			auto listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(LightDemo::onTouchesBegan, this);
			listener->onTouchEnded = CC_CALLBACK_2(LightDemo::onTouchesEnded, this);
			listener->onTouchMoved = CC_CALLBACK_2(LightDemo::onTouchMoved, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
			setPhysics3DDebugCamera(_camera);
			//initUI();
			Bref = true;
		}
	} while (0);
		return Bref;
}

void LightDemo::initUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto menuItem1 = MenuItemImage::create("button1.png", "button_1.png", CC_CALLBACK_1(LightDemo::onButtonClick, this));
	menuItem1->setTag(1);
	auto menuItem2 = MenuItemImage::create("button2.png", "button_2.png", CC_CALLBACK_1(LightDemo::onButtonClick, this));
	menuItem2->setTag(2);
	auto menuItem3 = MenuItemImage::create("button3.png", "button_3.png", CC_CALLBACK_1(LightDemo::onButtonClick, this));
	menuItem3->setTag(3);
	auto menuItem4 = MenuItemImage::create("button4.png", "button_4.png", CC_CALLBACK_1(LightDemo::onButtonClick, this));
	menuItem4->setTag(4);

	Menu* Tmenu = Menu::create(menuItem1, menuItem2, menuItem3, menuItem4, NULL);
	Tmenu->alignItemsHorizontallyWithPadding(3);//调整菜单项之间的距离
	Tmenu->setPosition(Point(visibleSize.width / 2, visibleSize.height - menuItem1->getContentSize().height / 2));
	Tmenu->setGlobalZOrder(3);
	Tmenu->setCameraMask(2);
	this->addChild(Tmenu, 1);
}

void LightDemo::onButtonClick(Ref* sender)
{
	if (((MenuItemImage *)sender)->getTag())
	{
		_player->setLightMask(0);


		if (((MenuItemImage *)sender)->getTag() == 2)
		{
			_player->setLightMask(-1);
			_ambientLight->setEnabled(true);
			_directionLight->setEnabled(false);
			_pointLight->setEnabled(false);
			_spotLight->setEnabled(false);
		}
		else if (((MenuItemImage *)sender)->getTag() == 3)
		{
			_player->setLightMask(-1);
			_ambientLight->setEnabled(false);
			_directionLight->setEnabled(true);
			_pointLight->setEnabled(false);
			_spotLight->setEnabled(false);
		}
		else if (((MenuItemImage *)sender)->getTag() == 4)
		{
			_player->setLightMask(-1);
			_ambientLight->setEnabled(false);
			_directionLight->setEnabled(false);
			_pointLight->setEnabled(true);
			_spotLight->setEnabled(false);
		}
		else
		{
			_player->setLightMask(0);
			_ambientLight->setEnabled(false);
			_directionLight->setEnabled(false);
			_pointLight->setEnabled(false);
			_spotLight->setEnabled(false);
		}
	}
}

bool LightDemo::onTouchesBegan(Touch* touch, Event* event)
{
	return true;
}

void LightDemo::onTouchMoved(Touch* touch, Event* event)
{
	auto addPoint = touch->getLocation() - touch->getStartLocation();
	float rotateAngle = addPoint.x *0.3;
	float camera_z=80*cos(rotateAngle*M_PI/180);
	float camera_x=80*sin(rotateAngle*M_PI/180);
	_camera->setPosition3D(Vec3(_player->getPosition3D()+Vec3(camera_x,80,camera_z)));
	_camera->lookAt(_player->getPosition3D(),Vec3(0,1,0));

	//auto startPoint = touch->getStartLocation();
	//Vec3 nearP(location.x, location.y, 0.0f), farP(location.x, location.y, 1.0f);

	_player->idle();
	
	
	
	
	//_camera->setRotation3D(Vec3(-45, rotateAngle, 0));

	//_layer3D->setRotation3D(Vec3(0, rotateAngle,0));
}

void LightDemo::onTouchesEnded(Touch* touch, Event* event)
{
	auto location = touch->getLocationInView();
	//_camera->setPosition3D(Vec3(0, 10080, 0));
	//_camera->setRotation3D(Vec3(-45, 0, 0));
	if (_camera)
	{
		_camera->setPosition3D(_player->getPosition3D() + Vec3(0, 80, 80));
		_camera->lookAt(_player->getPosition3D());
		if (_player)
		{
			Vec3 nearP(location.x, location.y, 0.0f), farP(location.x, location.y, 1.0f);
			auto size = Director::getInstance()->getWinSize();
			_layer3D->removeChildByTag(1);
			/*std::string fileName = "Sprite3DTest/girl.c3b";
			auto animation = Animation3D::create(fileName);
			if (animation)
			{
			auto animate = Animate3D::create(animation);
			_player->runAction(animate);
			}*/

			_camera->unproject(size, &nearP, &nearP);//获得射线的起点
			_camera->unproject(size, &farP, &farP);//获得射线的终点
			Vec3 dir = farP - nearP;//获得射线的向量
			dir.normalize();//向量单位化
			bool isInTerrain = _terrain->getIntersectionPoint(Ray(nearP, dir), collisionPoint);//判断是否在地形上，同时获得射线在地形的交点

			//_arrow->setCameraMask(2);
			//_arrow->setPosition3D(collisionPoint);
			// _arrow->setVisible(true);
			//auto blink = FadeOut::create(0.5f);
			//Blink* blink = Blink::create(0.5f, 3);
			//_arrow->runAction(blink);
			//_arrow->setVisible(false);
			ArmatureDataManager::getInstance()->addArmatureFileInfo("NewAnimation/NewAnimation.ExportJson");
			Armature* armature = Armature::create("NewAnimation");
			armature->getAnimation()->play("Animation1");
			_layer3D->addChild(armature);
			armature->setPosition3D(collisionPoint);
			armature->setAnchorPoint(Point::ZERO);
			armature->setCameraMask(2);
			armature->setScale(0.015f);
			armature->setTag(1);
			//_layer3D->removeChildByTag(1);

			if (!isInTerrain)
			{
				_player->idle();
				return;
			}
			dir = collisionPoint - _player->getPosition3D();
			dir.y = 0;
			dir.normalize();
			_player->_headingAngle = -1 * acos(dir.dot(Vec3(0, 0, -1)));
			dir.cross(dir, Vec3(0, 0, -1), &_player->_headingAxis);

			//this->schedule(schedule_selector(LightDemo::ensureWalk));

			_player->_targetPos = collisionPoint;
			_player->forward();
		}
	}
}

void LightDemo::ensureWalk(float bt)
{
	/*log("bt is %f", bt);
	Vec3 Normal;
	Vec3 _PlayerPoint = _player->getPosition3D();
	float Location_h = _terrain->getHeight(_PlayerPoint.x, _PlayerPoint.z, &Normal);
	float _terrainAxis;
	_terrainAxis = acos(Normal.dot(_player->_headingAxis));
	_player->_targetPos = collisionPoint;
	if (_terrainAxis >= (M_PI/3)|| _terrainAxis <= -(M_PI/3))
	{
		_player->back();
		return;
	}*/
}

