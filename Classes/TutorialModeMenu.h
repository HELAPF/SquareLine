#ifndef __TUTORIALMODEMENU_SCENE_H__
#define __TUTORIALMODEMENU_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <sstream>
#include <string>

class TutorialModeMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int);
	virtual bool init();
	virtual void update(float) override;
	void menuCloseCallback(cocos2d::Ref* pSender);
public:
	enum MENU_STATUS
	{
		STATE_SELECT_AVATAR1,
		STATE_SELECT_AVATAR2,
		STATE_SELECT_MAP,
	};

	enum ANIMATE_DIRECTION
	{
		ANIMATE_LEFT,
		ANIMATE_RIGHT,
		ANIMATE_NONE,
	};

	// Menu Creation
	void createAvatarMenu();
	void createAvatarButtons();
	void createAvatar(int,bool);
	void resetAvatar();

	void createMapMenu();
	void createMapButtons();
	void createMap(int, bool);

	void changeMenu(MENU_STATUS);

	// Menu Updates
	void handleMenuUpdates(float);
public:
	// Get|Set Level
	void setLevel(int i) { this->m_iLevel = i; }
	int getLevel() { return this->m_iLevel; }
private:
	MENU_STATUS m_eCurrentState;
	int m_iP1SelectedAvatar;
	int m_iP2SelectedAvatar;
	int m_iPreviousAvatar;
	int m_iSelectedMap;
	int m_iLevel;

	// Avatar Animation
	int m_iTotalAvatars;
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
	CREATE_FUNC(TutorialModeMenu);
};

#endif //
