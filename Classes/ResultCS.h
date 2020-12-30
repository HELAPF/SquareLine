#ifndef __ResultCS_
#define __ResultCS_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Classic_Ingame.h"
#include "Mode.h"

#include <iostream>
#include <sstream>
#include <string>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

class ResultCS : public cocos2d::Layer {
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background;
	Sprite *resultCube;

	string winner;
	int scoreGap;
	int remainTime;

	Label *winnerText;
	Label *scoreGapText;
	Label *remainTimeText;

	bool selected;

public:
	static cocos2d::Scene *createScene();
	virtual bool init();

	void CubeDrop(float delta);
	void ToggleToMenu();
	void update(float delta);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(ResultCS);
};

#endif