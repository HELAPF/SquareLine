#ifndef __Mode_
#define __Mode_

#define BGM "./FX/main_theme.wav"

#include "cocos2d.h"
#include "AvatarSelect.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class Mode : public cocos2d::Layer
{
private:
	enum MODE { CLASSIC, STAGE };

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *classicMode;
	Sprite *stageMode;

	Sprite *buttonLeft;
	Sprite *buttonRight;

	int nowSelected = CLASSIC;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void SelectedMode(float delta);
	void ChangeScene();
	CREATE_FUNC(Mode);
};

#endif