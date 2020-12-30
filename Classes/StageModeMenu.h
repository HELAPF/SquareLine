#ifndef __STAGEMODEMENU_SCENE_H__
#define __STAGEMODEMENU_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <sstream>
#include <string>

#include "Defines.h"

class StageModeMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float) override;
	void menuCloseCallback(cocos2d::Ref* pSender);
public:
	enum MENU_STATUS
	{
		STATE_SELECT_LEVEL,
		STATE_SELECT_STAGE,
	};
	enum DIFFICULTY
	{
		DIFF_NONE = 0,
		DIFF_EASY = 1,
		DIFF_NORMAL = 2,
		DIFF_HARD = 3,
	};
	enum ANIMATE_DIRECTION
	{
		ANIMATE_LEFT,
		ANIMATE_RIGHT,
		ANIMATE_NONE,
	};

	// Menu Creation
	void createLevelMenu();
	void createLevelButtons(Node*);
	void createStageMenu();
	void createStageButtons(Node*);
	void createMap(int, bool);
	void changeMenu(MENU_STATUS,DIFFICULTY = DIFF_NONE);
	void setBackground();

	// Menu Updates
	void handleMenuUpdates(float);

private:
	MENU_STATUS m_eCurrentState;
	DIFFICULTY m_eCurrentDifficulty;
	int m_iSelectedButton;
	int m_iSelectedLevel;

	// Map Animation
	ANIMATE_DIRECTION m_eAnimateDir;
	bool m_bAnimating;
	bool m_bSpawned;
public:
	// Listeners
	void createListeners();

	// Keyboard Listener
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event * event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event * event);

	// Mouse Listener
	void onMouseMove(cocos2d::Event * event);
	void onMouseUp(cocos2d::Event * event);
	void onMouseDown(cocos2d::Event * event);
	void onMouseScroll(cocos2d::Event * event);
public:
	CREATE_FUNC(StageModeMenu);
};

#endif // __STAGEMODEMENU_SCENE_H__
