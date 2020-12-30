#ifndef __MapSelect_
#define __MapSelect_

#define SELECT "FX/CrystalBell.wav"
#define DENY "FX/Buzz.wav"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "AvatarSelect.h"
#include "mapInit.h"
#include "Classic_Ingame.h"

USING_NS_CC;
using namespace CocosDenshion;

class MapSelect : public cocos2d::Layer {
private:
	Sprite *background;

	Sprite *cursor;
	Sprite *icon[5];
	bool map_Exist[5];
	int select;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(MapSelect);
};

#endif