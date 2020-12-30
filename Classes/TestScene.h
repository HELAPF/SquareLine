#ifndef __TESTSCENE_SCENE_H__
#define __TESTSCENE_SCENE_H__

#include "cocos2d.h"
#include <sstream>
#include <string>

class TestScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	// Keyboard Listener
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event * event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event * event);

    // implement the "static create()" method manually
	CREATE_FUNC(TestScene);
};

#endif // __TESTSCENE_SCENE_H__
