#ifndef _IMPORTSCENE_H_
#define  _IMPORTSCENE_H_
#include"cocos2d.h"
USING_NS_CC;
class importscene: public Layer
{
public:
	static Scene* createScene();
	bool init();
	CREATE_FUNC(importscene);
protected:
private:
	Camera* _camera;
};
#endif