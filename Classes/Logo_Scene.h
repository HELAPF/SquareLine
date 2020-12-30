#ifndef __Logo_Scene_
#define __Logo_Scene_

#include "cocos2d.h"
#include "Start.h"

USING_NS_CC;

class Logo_Scene : public cocos2d::Layer
{
private :
	Sprite *background;
	Sprite *logo_sprite;

public :
	static cocos2d::Scene* createScene();
	virtual bool init();

	void SceneChange(float delta);
	CREATE_FUNC(Logo_Scene);
};

#endif