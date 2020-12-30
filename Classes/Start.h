#ifndef __Test_
#define __Test_

#include "cocos2d.h"
#include "Mode.h"
#include "SimpleAudioEngine.h"

#define FX_INCORRECT "FX/Buzz.wav"
#define BGM "./FX/main_theme.wav"

USING_NS_CC;
using namespace CocosDenshion;

class Start : public cocos2d::Layer
{
private:
	enum buttonStatus { START, EXIT };
	int nowSelected = 0;

public:
	Label *startButton;
	Label *optionButton;
	Label *exitButton;

	static cocos2d::Scene* createScene();

	virtual bool init();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void SelectedButton(float delta);

	void ChangeScene(int type);
	CREATE_FUNC(Start);
};

#endif