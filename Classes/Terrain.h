#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include"cocos2d.h"
USING_NS_CC;
class terrain :public Node
{
public:
	static Scene* createScene();
	bool init();
	CREATE_FUNC(terrain);
public:
	void createWorld3D();
	terrain();
	~terrain();
private:
	Terrain* _terrain;
	Skybox* _skyBox;
	Camera* _camera;
	Sprite3D* _player;
	TextureCube* _textureCube;
	Animate3D* _swim;
	Animate3D* _hurt;
	Layer* _WorldScene;
	std::vector<Camera*>_gameCameras;

	enum GAME_CAMERAS_ORDER
	{
		CAMERA_WORLD_3D_SKYBOX = 0,
		CAMERA_WORLD_3D_SCENE,
		CAMERA_UI_2D,
		CAMERA_DIALOG_2D_BASE,
		CAMERA_DIALOG_3D_MODEL,
		CAMERA_DIALOG_2D_ABOVE,
		CAMERA_OSD_2D_BASE,
		CAMERA_OSD_3D_MODEL,
		CAMERA_OSD_2D_ABOVE,
		CAMERA_COUNT,
	};
	enum GAME_SCENE {
		SCENE_UI = 0,
		SCENE_WORLD,
		SCENE_DIALOG,
		SCENE_OSD,
		SCENE_COUNT,
	};

	/** Define the layers in scene, layer seperated by camera mask. */
	enum SCENE_LAYER {
		LAYER_BACKGROUND = 0,
		LAYER_DEFAULT,
		LAYER_MIDDLE,
		LAYER_TOP,
		LAYER_COUNT,
	};
	//static CameraFlag s_CF[LAYER_COUNT];
	//static unsigned short s_CM[LAYER_COUNT];
	//static const char * s_CameraNames[CAMERA_COUNT];
	static Vec3 s_scenePositons[SCENE_COUNT];
};
#endif