#ifndef __GAMEBOARD_H__
#define __GAMEBOARD_H__

class BoardPiece;

#include "cocos2d.h"

#include "Defines.h"

USING_NS_CC;

class GameBoard : public cocos2d::Node
{
public:
	enum PIECE_POSITION
	{
		PP_MIDDLE,
		PP_TOP,
		PP_TOPLEFT,
		PP_TOPRIGHT,
		PP_BOTTOM,
		PP_BOTTOMLEFT,
		PP_BOTTOMRIGHT,
		PP_LEFT,
		PP_RIGHT,
		PP_TOTAL,
		PP_NONE = -1,
	};
	enum BOARD_TYPE
	{
		TYPE_NORMAL,
		TYPE_CIRCLE,
		TYPE_HEART,
		TYPE_ACORN,
		TYPE_CLOVER,
	};

	GameBoard(BOARD_TYPE bt = TYPE_NORMAL);
	~GameBoard();

	virtual void Update();

	void createObstacles();
	void Reset();
public:
	// Get
	BoardPiece * getBoardPiece(int i);
	BoardPiece * getBoardPieceByDirection(PIECE_POSITION,int,Vec2);
	Vec2 getUpdatedGridIndex(PIECE_POSITION,Vec2, int);
	Vec2 getGridIndex(Vec2,int);

private:
	BoardPiece * m_cBoardPieces[9]; // 9 Pieces, 4x4 each
	BOARD_TYPE m_eBoardType;
};

#endif