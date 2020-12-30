#ifndef __TUTORIAL_MODE_GAME_H__
#define __TUTORIAL_MODE_GAME_H__

#include "cocos2d.h"
#include "GameBoard.h"
#include "BoardPiece.h"
#include "StageModePlayer.h"
#include "StageModeNPC.h"
#include "Mission.h"
#include "Timer.h"

class TutorialModeMenu;

class TutorialModeGame : public cocos2d::Layer
{
public:
	enum EASY_STAGE
	{
		EASY_1 = 1,
		EASY_2,
		EASY_3,
	};

	enum EASY_STEPS
	{
		STEP_1_0 = 0,
		STEP_1_1 = 1,
		STEP_1_2 = 2,
		STEP_1_3 = 3,
		STEP_1_4 = 4,
		STEP_1_5 = 5,
		STEP_1_6 = 6,
		STEP_1_7 = 7,
		STEP_1_8 = 8,
		STEP_1_9 = 9,
		STEP_1_10 = 10,
		STEP_1_11 = 11,
		STEP_1_12 = 12,
		STEP_1_13 = 13,
		STEP_1_14 = 14,
		STEP_1_15 = 15,
		STEP_1_16 = 16,
		STEP_1_17 = 17,
		STEP_1_18 = 18,
		STEP_1_19 = 19,
		STEP_1_20 = 20,
		STEP_1_21 = 21,
		STEP_1_22 = 22,
		STEP_1_23 = 23,
		STEP_1_24 = 24,
		STEP_1_25 = 25,
		STEP_1_26 = 26,
		STEP_1_27 = 27,
		STEP_1_28 = 28,
		STEP_1_TOTAL = 29,
		STEP_2_0 = STEP_1_TOTAL+1,
		STEP_2_1,
		STEP_2_2,
		STEP_2_3,
		STEP_2_4,
		STEP_2_5,
		STEP_2_6,
		STEP_2_TOTAL,
		STEP_3_0 = STEP_2_TOTAL + 1,
		STEP_3_TOTAL,
	};

	// Score Rewards
	enum SCORE_REWARD
	{
		SCORE_REWARD_1,
		SCORE_REWARD_2,
		SCORE_REWARD_3,
		SCORE_REWARD_TOTAL
	};

	static cocos2d::Scene* createScene(int,int,int,int);
	virtual bool init();
	virtual void update(float) override;
	void menuCloseCallback(cocos2d::Ref* pSender);
public:
	// GameBoard 
	virtual void createBoard();

	// UI
	virtual void createUI();
	virtual Sprite * createAvatar(int);
	virtual void createGameOverMenu(std::string);

	// Gameplay
	virtual void spawnPlayer(int);
	virtual void spawnEnemy(int);
	virtual void checkScore();
	virtual void updateScore();

	// Tutorial
	void createInactiveTile(int,int,GameBoard::PIECE_POSITION,bool);

public:
	// Get|Set
	void setStage(EASY_STAGE);
	EASY_STAGE getStage();

	std::string getStepText(EASY_STEPS);
private:
	// GameBoard
	GameBoard * m_cGameBoard;

	// Mission
	Mission * m_cMission;

	// Information
	Vec2 m_v2Player1PositionIndex;
	GameBoard::PIECE_POSITION m_ePlayer1PP;
	bool m_bPlayer1Pick;
	Vec2 m_v2Player2PositionIndex;
	GameBoard::PIECE_POSITION m_ePlayer2PP;
	bool m_bPlayer2Pick;
	bool m_bReward1;
	bool m_bReward2;
	bool m_bReward3;

	// Stage
	EASY_STAGE m_eStage;
	
	// Steps
	EASY_STEPS m_eStep;
	bool m_bInStep;

	// Key Check
	bool m_bKeyPressed;

	// Player
	StageModePlayer * m_cPlayer;

	// NPC
	StageModeNPC * m_cNPC;

	// Timer
	StageModeTimer * m_cTimer;

	// Game Over
	bool m_bShowGameOver;

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
	CREATE_FUNC(TutorialModeGame);
};

#endif