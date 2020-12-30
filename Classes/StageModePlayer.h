#ifndef __STAGE_MODE_PLAYER_H__
#define __STAGE_MODE_PLAYER_H__

#include "cocos2d.h"
#include <string>
#include "GameBoard.h"
#include "Mission.h"

class Grid;

USING_NS_CC;

class StageModePlayer : public cocos2d::Node
{
public:
	enum PLAYER_DIRECTION
	{
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		DIRECTION_NONE,
	};

public:
	StageModePlayer(const std::string);
	~StageModePlayer();

	void printDebugInfo();
public:
	// Getters and Setters
	void setName(const std::string);
	std::string getName();

	void setPosition(Vec2);
	Vec2 getPosition();

	void setIndex(Vec2);
	Vec2 getIndex();

	void setPiecePosition(GameBoard::PIECE_POSITION);
	GameBoard::PIECE_POSITION getPiecePosition();

	void setFirstMove(bool);
	bool getFirstMove();

	void setScore(int);
	int getScore();
	void incrementScoreBy(int);

	void setLinesMade(int);
	int getLinesMade();

	void setPreviousLines(int);
	int getPreviousLines();

	void setOldGrid(Grid*);
	Grid * getOldGrid();

	void setOldIndex(Vec2);
	Vec2 getOldIndex();

	Sprite * getSprite();

	void setAvatarID(int);
	int getAvatarID();

	void setStunDuration(float);
	float getStunDuration();

	void setPreviousDirection(PLAYER_DIRECTION);
	PLAYER_DIRECTION getPreviousDirection();

	// Movement
	void toggleCursor();
	void MoveUp(GameBoard*);
	void MoveDown(GameBoard*);
	void MoveLeft(GameBoard*);
	void MoveRight(GameBoard*);
	void Reset(GameBoard*);

	// Cloning
	Sprite * createClone(Vec2);

	// Scoring
	bool checkScoringTile(GameBoard*,Mission*);
private:
	std::string m_sPlayerName;
	Vec2 m_v2PlayerPos;
	Vec2 m_v2PlayerIndex;
	GameBoard::PIECE_POSITION m_ePlayerPP;
	bool m_bCursorToggled;
	Vector<Grid*> m_cSelectedGrids;
	bool m_bFirstMove;
	int m_iPlayerScore;
	int m_iLinesMade;
	int m_iPreviousLines;
	int m_iAvatarID;

	Grid * m_cOldGrid;
	Vec2 m_v2OldIndex;

	PLAYER_DIRECTION m_ePreviousDirection;

	float m_fStunDuration;
};

#endif