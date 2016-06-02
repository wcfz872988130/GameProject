#include "collision.h"
#include "3d/CCTerrain.h"
#include "3d/CCBundle3D.h"
#include "physics3d/CCPhysics3D.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;



Scene* Physics3DTerrainDemo::createScene()
{
	Scene* scene=NULL;
	do 
	{
		scene = Scene::createWithPhysics();
		CC_BREAK_IF(!scene);
		auto layer = Physics3DTerrainDemo::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool Physics3DTerrainDemo::init()
{
	if (initWithPhysics())
	{
		getPhysics3DWorld()->setDebugDrawEnable(false);
		Size visiblesize = Director::getInstance()->getWinSize();
		_camera = Camera::createPerspective(30.0f, visiblesize.width / visiblesize.height, 1.0f, 1000.0f);
		_camera->setPosition3D(Vec3(0.0f,50.0f,100.0f));
		_camera->lookAt(Vec3(0.0f,0.0f,0.0f),Vec3(0.0f,1.0f,0.0f));
		_camera->setCameraFlag(CameraFlag::USER1);
		this->addChild(_camera);
		this->addChild(physicsScene);
		//Physics3DRigidBodyDes rbDes;
		Terrain::DetailMap r("TerrainTest/dirt.jpg"), g("TerrainTest/Grass2.jpg", 10), b("TerrainTest/road.jpg"), a("TerrainTest/GreenSkin.jpg", 20);
		Terrain::TerrainData data("TerrainTest/heightmap129.jpg", "TerrainTest/alphamap.png", r, g, b, a, Size(32, 32), 20.0f, 1.0f);
		auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
		terrain->setMaxDetailMapAmount(4);
		terrain->setCameraMask(2);
		terrain->setDrawWire(false);
		terrain->setSkirtHeightRatio(3);
		terrain->setLODDistance(64, 128, 192);
		terrain->setCameraMask((unsigned short)CameraFlag::USER1);
		std::vector<float>heidata = terrain->getHeightData();
		auto size = terrain->getTerrainSize();
		Physics3DColliderDes colliderDes;
		colliderDes.shape = Physics3DShape::createHeightfield(size.width, size.height, &heidata[0], 1.0f, terrain->getMinHeight(), terrain->getMaxHeight(), true, false, true);
		auto collider = Physics3DCollider::create(&colliderDes);
		auto component = Physics3DComponent::create(collider);
		terrain->addComponent(component);
		physicsScene->addChild(terrain);
		component->syncNodeToPhysics();
		component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);

		//create Mesh
		std::vector<Vec3> triangleList = Bundle3D::getTrianglesList("Sprite3DTest/boss.c3b");
		colliderDes.shape = Physics3DShape::createMesh(&triangleList[0], (int)triangleList.size() / 3);
		auto sprite = PhysicsSprite3D::createWithCollider("Sprite3DTest/boss.c3b", &colliderDes);
		sprite->setRotation3D(Vec3(-90.0f, 0.0f, 0.0f));
		sprite->setPosition3D(Vec3(0.0f, 15.0f, 0.0f));
		sprite->setCameraMask(2);
		physicsScene->addChild(sprite);
		sprite->syncNodeToPhysics();
		sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
		std::vector<std::pair<Physics3DShape*, Mat4>>shapeList;
		{
			Mat4 localTrans;
			auto bodyshape = Physics3DShape::createBox(Vec3(2.0f, 4.0f, 2.0f));
			Mat4::createTranslation(0.0f, 2.0f, 0.0f, &localTrans);
			shapeList.push_back(std::make_pair(bodyshape, localTrans));
			auto headshape = Physics3DShape::createSphere(1.5f);
			Mat4::createTranslation(0.6f, 5.0f, -1.5f, &localTrans);
			shapeList.push_back(std::make_pair(headshape, localTrans));
			auto Ihandshape = Physics3DShape::createBox(Vec3(1.0f, 3.0f, 1.0f));
			Mat4::createRotation(Vec3(1.0f, 0.0f, 0.0f), CC_DEGREES_TO_RADIANS(15.0f), &localTrans);
			localTrans.m[12] = -1.5f;
			localTrans.m[13] = 2.5f;
			localTrans.m[14] = -2.5f;
			shapeList.push_back(std::make_pair(Ihandshape, localTrans));
			auto rhandshape = Physics3DShape::createBox(Vec3(1.0f, 3.0f, 1.0f));
			Mat4::createRotation(Vec3(1.0f, 0.0f, 0.0f), CC_DEGREES_TO_RADIANS(-15.0f), &localTrans);
			localTrans.m[12] = 2.0f;
			localTrans.m[13] = 2.5f;
			localTrans.m[14] = 1.0f;
			shapeList.push_back(std::make_pair(rhandshape, localTrans));

			/*rbDes.mass = 10.0f;
			rbDes.shape = Physics3DShape::createCompoundShape(shapeList);
			auto rigidBody = Physics3DRigidBody::create(&rbDes);
			component = Physics3DComponent::create(rigidBody);
			auto sprite = Sprite3D::create("Sprite3DTest/orc.c3b");
			sprite->addComponent(component);
			sprite->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
			sprite->setPosition3D(Vec3(-5.0f, 20.0f, 0.0f));
			sprite->setScale(0.4f);
			sprite->setCameraMask(2);
			this->addChild(sprite);*/
		}
//		physicsScene->setPhysics3DDebugCamera(_camera);
	}
	return true;
}
std::string Physics3DTerrainDemo::subtitle() const
{
	return "Physics3D CollisionCallback";
}