#ifndef __STAGE_MODE_GAME_H__
#define __STAGE_MODE_GAME_H__

#include "cocos2d.h"

#include "GameBoard.h"
#include "BoardPiece.h"
#include "StageModePlayer.h"
#include "StageModeNPC.h"
#include "Mission.h"
#include "Timer.h"

class StageModeMenu;

class StageModeGame : public cocos2d::Layer
{
public:
	// NORMAL AND HARD STAGES
	enum STAGE
	{
		STAGE_NORMAL_1 = 1,
		STAGE_NORMAL_2,
		STAGE_NORMAL_3,
		STAGE_NORMAL_4,
		STAGE_HARD_1,
		STAGE_HARD_2,
		STAGE_HARD_3,
		STAGE_TOTAL,
	};

	// Score Rewards
	enum SCORE_REWARD
	{
		SCORE_REWARD_1,
		SCORE_REWARD_2,
		SCORE_REWARD_3,
		SCORE_REWARD_TOTAL
	};

public:
	static cocos2d::Scene* createScene(int level, int diff);
	virtual bool init();
	virtual void update(float) override;
	void menuCloseCallback(cocos2d::Ref* pSender);
public:
	// GameBoard 
	virtual void createBoard(int,int);
	virtual void setBackground(int, int);

	// UI
	virtual void createUI();
	virtual Sprite * createAvatar(int);
	virtual void createGameOverMenu(std::string);

	// Gameplay
	virtual void spawnPlayer(int,int);
	virtual void spawnEnemy(int,int);
	virtual void checkScore();
	virtual void updateScore();

public:
	void setStage(int,int);
	STAGE getStage();
	std::string getStepText(STAGE);
private:
	// GameBoard
	GameBoard * m_cGameBoard;

	// Mission
	Mission * m_cMission;

	// Information
	Vec2 m_v2PlayerPositionIndex;
	GameBoard::PIECE_POSITION m_ePlayerPP;
	Vec2 m_v2EnemyPositionIndex;
	GameBoard::PIECE_POSITION m_eEnemyPP;
	bool m_bReward1;
	bool m_bReward2;
	bool m_bReward3;
	
	// Stage
	STAGE m_eStage;

	// Gameplay
	bool m_bCursorToggled;

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
	CREATE_FUNC(StageModeGame);
};

#endif