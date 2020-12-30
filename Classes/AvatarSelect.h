#ifndef __AvatarSelect_
#define __AvatarSelect_

#define SELECT "FX/CrystalBell.wav"
#define DENY "FX/Buzz.wav"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Mode.h"
#include "MapSelect.h"

USING_NS_CC;
using namespace CocosDenshion;

class AvatarSelect : public cocos2d::Layer {
private:
	Sprite *background;
	Sprite *tile;
	Label *message;

	bool avatar_Exist[12];
	Sprite *avatarList[12];

	Sprite *p1_Cursor;
	Sprite *p1_Select_Bubble;
	int p1_Select;
	bool is_p1_Selected;

	Sprite *p2_Cursor;
	Sprite *p2_Select_Bubble;
	int p2_Select;
	bool is_p2_Selected;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void allPickCheck();

	CREATE_FUNC(AvatarSelect);
};

#endif