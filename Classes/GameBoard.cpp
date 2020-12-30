#include "GameBoard.h"
#include "BoardPiece.h"
#include "Grid.h"

GameBoard::GameBoard(BOARD_TYPE bt)
: m_eBoardType(bt)
{
	auto background = Sprite::create("Background.png");
	background->setScale(1.05f * GLOBAL_UI_SCALE);
	background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.5f)));
	background->setName("background");
	//this->addChild(background, -1);

	float gridOffset = 0.f;

	for (unsigned int i = 0; i < 9; i++)
	{
		m_cBoardPieces[i] = new BoardPiece((PIECE_POSITION)(i), this->m_eBoardType);

		switch (PIECE_POSITION(i))
		{
		case PP_MIDDLE:
			m_cBoardPieces[i]->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.5f)));
			break;
		case PP_TOP:
			m_cBoardPieces[i]->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset)); // 0.5 offset
			break;
		case PP_TOPLEFT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset));
			break;
		case PP_TOPRIGHT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset));
			break;
		case PP_BOTTOM:
			m_cBoardPieces[i]->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset));
			break;
		case PP_BOTTOMLEFT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset));
			break;
		case PP_BOTTOMRIGHT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset));
			break;
		case PP_LEFT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) - m_cBoardPieces[i]->getPieceSize() - gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));
			break;
		case PP_RIGHT:
			m_cBoardPieces[i]->setPosition(Vec2((Director::getInstance()->getVisibleSize().width * 0.5f) + m_cBoardPieces[i]->getPieceSize() + gridOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));
			break;
		}
		this->addChild(m_cBoardPieces[i], -1);
	}
}

GameBoard::~GameBoard()
{
	// Cleanup
	for (BoardPiece * p : m_cBoardPieces)
	{
		if (p)
			delete p;
	}
}

void GameBoard::Update()
{
	for (int i = 0; i < 8; i++)
	{
		this->m_cBoardPieces[i]->Update();
	}
}

BoardPiece * GameBoard::getBoardPiece(int i)
{
	return this->m_cBoardPieces[i];
}

BoardPiece * GameBoard::getBoardPieceByDirection(GameBoard::PIECE_POSITION pp, int dir,Vec2 playerIndex)
{
	switch (pp)
	{
	case PP_MIDDLE:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOP);
			else if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_LEFT);
			else
				return this->getBoardPiece(PP_TOPLEFT);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_TOP);
			break;
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOP);
			else if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_RIGHT);
			else
				return this->getBoardPiece(PP_TOPRIGHT);
			break;
		case 3: // Left
			return this->getBoardPiece(PP_LEFT);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_RIGHT);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOM);
			else if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_LEFT);
			else
				return this->getBoardPiece(PP_BOTTOMLEFT);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_BOTTOM);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOM);
			else if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_RIGHT);
			else
				return this->getBoardPiece(PP_BOTTOMRIGHT);
			break;
		}
		break;
	case PP_TOP:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_TOPLEFT);
			break;
		case 2: // Top Right
			if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_TOPRIGHT);
			break;
		case 3: // Left
			return this->getBoardPiece(PP_TOPLEFT);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_TOPRIGHT);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_MIDDLE);
			else if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_TOPLEFT);
			else
				return this->getBoardPiece(PP_LEFT);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_MIDDLE);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_MIDDLE);
			else if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_TOPRIGHT);
			else
				return this->getBoardPiece(PP_RIGHT);
			break;
		}
		break;
	case PP_TOPLEFT:
		switch (dir)
		{
		case 2: // Top Right
			if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_TOP);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_TOP);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_LEFT);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_LEFT);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_LEFT);
			else if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_TOP);
			else
				return this->getBoardPiece(PP_MIDDLE);
			break;
		}
		break;
	case PP_TOPRIGHT:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_TOP);
				break;
		case 3: // Left
			return this->getBoardPiece(PP_TOP);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_RIGHT);
			else if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_TOP);
			else
				return this->getBoardPiece(PP_MIDDLE);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_RIGHT);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_RIGHT);
			break;
		}
		break;
	case PP_BOTTOM:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_MIDDLE);
			else if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_BOTTOMLEFT);
			else
				return this->getBoardPiece(PP_LEFT);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_MIDDLE);
			break;
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_MIDDLE);
			else if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_BOTTOMRIGHT);
			else
				return this->getBoardPiece(PP_RIGHT);
			break;
		case 3: // Left
			return this->getBoardPiece(PP_BOTTOMLEFT);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_BOTTOMRIGHT);
			break;
		case 5: // Bottom Left
			if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_BOTTOMLEFT);
			break;
		case 7: // BottomRight
			if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_BOTTOMRIGHT);
			break;
		}
		break;
	case PP_BOTTOMLEFT:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_LEFT);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_LEFT);
			break;
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_LEFT);
			else if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_BOTTOM);
			else
				return this->getBoardPiece(PP_MIDDLE);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_BOTTOM);
			break;
		case 7: // BottomRight
			if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_BOTTOM);
			break;
		}
		break;
	case PP_BOTTOMRIGHT:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_RIGHT);
			else if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_BOTTOM);
			else
				return this->getBoardPiece(PP_MIDDLE);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_RIGHT);
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_RIGHT);
			break;
		case 3: // Left
			return this->getBoardPiece(PP_BOTTOM);
			break;
		case 5: // Bottom Left
			if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_BOTTOM);
			break;
		}
		break;
	case PP_LEFT:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOPLEFT);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_TOPLEFT);
			break;
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOPLEFT);
			else if (playerIndex.x == 3 && playerIndex.y > 0)
				return this->getBoardPiece(PP_MIDDLE);
			else
				return this->getBoardPiece(PP_TOP);
			break;
		case 4: // Right
			return this->getBoardPiece(PP_MIDDLE);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOMLEFT);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_BOTTOMLEFT);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOMLEFT);
			else if (playerIndex.x == 3 && playerIndex.y < 3)
				return this->getBoardPiece(PP_MIDDLE);
			else
				return this->getBoardPiece(PP_BOTTOM);
			break;
		}
		break;
	case PP_RIGHT:
		switch (dir)
		{
		case 0: // Top Left
			if (playerIndex.x > 0 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOPRIGHT);
			else if (playerIndex.x == 0 && playerIndex.y > 0)
				return this->getBoardPiece(PP_MIDDLE);
			else
				return this->getBoardPiece(PP_TOP);
			break;
		case 1: // Top
			return this->getBoardPiece(PP_TOPRIGHT);
			break;
		case 2: // Top Right
			if (playerIndex.x < 3 && playerIndex.y == 0)
				return this->getBoardPiece(PP_TOPRIGHT);
			break;
		case 3: // Left
			return this->getBoardPiece(PP_MIDDLE);
			break;
		case 5: // Bottom Left
			if (playerIndex.x > 0 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOMRIGHT);
			else if (playerIndex.x == 0 && playerIndex.y < 3)
				return this->getBoardPiece(PP_MIDDLE);
			else
				return this->getBoardPiece(PP_BOTTOM);
			break;
		case 6: // Bottom
			return this->getBoardPiece(PP_BOTTOMRIGHT);
			break;
		case 7: // BottomRight
			if (playerIndex.x < 3 && playerIndex.y == 3)
				return this->getBoardPiece(PP_BOTTOMRIGHT);
			break;
		}
		break;
	}
	return nullptr;
}

Vec2 GameBoard::getGridIndex(Vec2 playerIndex, int index)
{
	std::stringstream ss;
	Vec2 gridIndex = Vec2(-1,-1);
	switch (index)
	{
	case 0: // Top Left
		gridIndex = Vec2(playerIndex.x-1, playerIndex.y-1);
		break;
	case 1: // Top
		gridIndex = Vec2(playerIndex.x, playerIndex.y-1);
		break;
	case 2: // Top Right
		gridIndex = Vec2(playerIndex.x+1, playerIndex.y-1);
		break;
	case 3: // Left
		gridIndex = Vec2(playerIndex.x-1, playerIndex.y);
		break;
	case 4: // Right
		gridIndex = Vec2(playerIndex.x+1, playerIndex.y);
		break;
	case 5: // Bottom Left
		gridIndex = Vec2(playerIndex.x-1, playerIndex.y+1);
		break;
	case 6: // Bottom
		gridIndex = Vec2(playerIndex.x, playerIndex.y+1);
		break;
	case 7: // BottomRight
		gridIndex = Vec2(playerIndex.x+1, playerIndex.y+1);
		break;
	default:
		ss << "How did you get here!? getGridIndex() default : Index(" <<  index << ")";
		MessageBox(ss.str().c_str(),"ERROR");
		break;
	}
	return gridIndex;
}

Vec2 GameBoard::getUpdatedGridIndex(PIECE_POSITION pp, Vec2 playerIndex, int dir)
{
	// TO BE FIXED
	switch (dir)
	{
	case 0: // Top Left
		if (playerIndex.x > 0 && playerIndex.y == 0)
			return Vec2(playerIndex.x - 1, 3);
		else if (playerIndex.x == 0 && playerIndex.y > 0)
			return Vec2(3, playerIndex.y - 1);
		else
			return Vec2(3, 3);
		break;
	case 1: // Top
		return Vec2(playerIndex.x, 3);
		break;
	case 2: // Top Right
		if (playerIndex.x < 3 && playerIndex.y == 0)
			return Vec2(playerIndex.x + 1, 3);
		else if (playerIndex.x == 3 && playerIndex.y > 0)
			return Vec2(0, playerIndex.y - 1);
		else
			return Vec2(0, 3);
		break;
	case 3: // Left
		return Vec2(3, playerIndex.y);
		break;
	case 4: // Right
		return Vec2(0, playerIndex.y);
		break;
	case 5: // Bottom Left
		if (playerIndex.x > 0 && playerIndex.y == 3)
			return Vec2(playerIndex.x - 1, 0);
		else if (playerIndex.x == 0 && playerIndex.y < 3)
			return Vec2(3 ,playerIndex.y + 1);
		else
			return Vec2(3, 0);
		break;
	case 6: // Bottom
		return Vec2(playerIndex.x, 0);
		break;
	case 7: // BottomRight
		if (playerIndex.x < 3 && playerIndex.y == 3)
			return Vec2(playerIndex.x + 1, 0);
		else if (playerIndex.x == 3 && playerIndex.y < 3)
			return Vec2(0, playerIndex.y + 1);
		else
			return Vec2(0, 0);
		break;
	}

	return Vec2(-1, -1);
}

void GameBoard::Reset()
{
	for (BoardPiece * m : m_cBoardPieces)
	{
		m->Reset(this->m_eBoardType);
	}
}

void GameBoard::createObstacles()
{
	for (BoardPiece * bp : m_cBoardPieces)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (!bp->getGrid(i, j)->getCloned())
				{
					bp->getGrid(i, j)->SpawnObstacle();
				}
			}
		}
	}
}