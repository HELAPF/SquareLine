#ifndef __tempScene_
#define __tempScene_

#include "cocos2d.h"
#include "ResultCS.h"
#include "Classic_Ingame.h"

#define BLACKTILE "UI/result/tileb.png"
#define WHITETILE "UI/result/tilew.png"

USING_NS_CC;

class tempScene : public cocos2d::Layer {
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background;
	Sprite *tile[10][6];

public:
	static cocos2d::Scene *createScene();
	virtual bool init();

	void changeScene(float delta);

	CREATE_FUNC(tempScene);
};

#endif