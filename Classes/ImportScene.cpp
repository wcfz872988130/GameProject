#include "ImportScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

Scene* importscene::createScene()
{
	Scene* scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(!scene);
		auto layer = importscene::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}
bool importscene::init()
{
	/*导入场景*/
	auto rootNode = CSLoader::createNode("Scene3D.csb");
	this->addChild(rootNode);

	// 获取摄像机
	_camera = (Camera*)rootNode->getChildByName("UserCamera_1");
	_camera->setCameraFlag(CameraFlag::USER1);
	//_camera->setDepth(1000);

	// 获取模型
	auto _sprite3D = static_cast<Sprite3D*>(rootNode->getChildByName("Aurora"));
	//_sprite3D->setCameraMask(2);
	//_sprite3D->setLightMask(0);
	_sprite3D->setScale(1);
	return true;
}