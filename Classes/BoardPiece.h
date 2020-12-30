#ifndef __BOARDPIECE_H__
#define __BOARDPIECE_H__

#include "cocos2d.h"

USING_NS_CC;

class Grid;

#include "GameBoard.h"

class BoardPiece : public cocos2d::Node
{
public:
	BoardPiece(GameBoard::PIECE_POSITION,GameBoard::BOARD_TYPE);
	~BoardPiece();
	virtual void Update();

public:
	// General Functions
	void Init(GameBoard::BOARD_TYPE);
	void removeSpecificPieces(GameBoard::BOARD_TYPE);
	void setAllInactive();
	void setAllActive();
	void createPairAtLocation(Vec2, Vec2);
	void Reset(GameBoard::BOARD_TYPE);

	// Set and Get
	void setPieceSize(float size) { this->m_fPieceSize = size; }
	float getPieceSize() { return this->m_fPieceSize; }

	GameBoard::PIECE_POSITION getPositionID() { return this->m_ePositionID; }

	Grid * getGrid(int x, int y) { return this->m_cArea[y][x]; }

	int getTotalActive() { return this->m_iTotalActive; }
private:
	Grid * m_cArea[4][4]; // Individual 4x4 Pieces
	GameBoard::PIECE_POSITION m_ePositionID;
	float m_fPieceSize;
	int m_iPopulationCount;
	int m_iTotalActive;
};

#endif