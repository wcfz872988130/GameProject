#include "Terrain.h"

terrain::terrain()
{
}

terrain::~terrain()
{
}

Scene* terrain::createScene()
{
	Scene* scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(!scene);
		auto layer = Layer::create();
		CC_BREAK_IF(!layer);
	} while (0);
	return scene;
}

bool terrain::init()
{
	bool Bref = false;
	do 
	{

		auto visibleSize = Director::getInstance()->getVisibleSize();
		//_gameCameras.resize(CAMERA_COUNT);
		Camera* ca = nullptr;
		_WorldScene = Layer::create();

		createWorld3D();
		_WorldScene->addChild(_skyBox);
		_WorldScene->addChild(_terrain);

		/*创建一个观察天空盒的摄像机*/
		ca =  Camera::createPerspective(60, visibleSize.width / visibleSize.height, 10, 100);
		ca->setDepth(0);//////////
		//ca->setName(s_CameraNames[CAMERA_WORLD_3D_SKYBOX]);////
		ca->setCameraFlag(CameraFlag::USER1);
		//ca->setCameraFlag(s_CF[LAYER_BACKGROUND]);
		ca->setPosition3D(Vec3(0.f,0.f,500.f));
		ca->lookAt(Vec3(50, 0, 10));
		_camera = ca;
		_WorldScene->addChild(ca);

		this->addChild(_WorldScene,0);
		Bref = true;
	} while (0);
	return Bref;
}

void terrain::createWorld3D()
{
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

	state->setUniformTexture("u_cubeTex",_textureCube);

	/*创建天空盒*/
	_skyBox = Skybox::create();
	//_skyBox->setCameraMask(s_CM[LAYER_BACKGROUND]);
	_skyBox->setCameraMask(2);
	_skyBox->setTexture(_textureCube);
	_skyBox->setScale(700.f);

	/*创建地形*/
	Terrain::DetailMap r("terrain/dirt.jpg");
	Terrain::DetailMap g("terrain/Grass2.jpg",10);
	Terrain::DetailMap b("terrain/road.jpg");
	Terrain::DetailMap a("terrain/GreenSkin.jpg");
	//Terrain::TerrainData data("terrain/heightmap16.jpg","terrain/alphamap.png",r,g,b,a,Size(32,32),40.0f,2);
	Terrain::TerrainData data("TerrainTest/heightmap16.jpg", "TerrainTest/alphamap.png",r, g, b, a);
	_terrain = Terrain::create(data,Terrain::CrackFixedType::SKIRT);
	_terrain->setMaxDetailMapAmount(4);
	_terrain->setDrawWire(false);
	_terrain->setCameraMask(2);
	_terrain->setSkirtHeightRatio(3);
	_terrain->setLODDistance(64, 128, 192);

}
