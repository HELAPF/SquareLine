#include "StageModePlayer.h"

#include "BoardPiece.h"
#include "Grid.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

StageModePlayer::StageModePlayer(const std::string playerName)
: m_sPlayerName(playerName)
, m_v2PlayerPos(Vec2::ZERO)
, m_v2PlayerIndex(Vec2::ZERO)
, m_ePlayerPP(GameBoard::PP_NONE)
, m_bCursorToggled(false)
, m_bFirstMove(true)
, m_iPlayerScore(0)
, m_iLinesMade(0)
, m_iPreviousLines(0)
, m_iAvatarID(0)
, m_ePreviousDirection(DIRECTION_NONE)
{
	cocos2d::Node::setName("Player");
	auto playerSprite = Sprite::create("Cursors/PlayerCursor.png");
	playerSprite->setName("Player Sprite");
	playerSprite->setScale(1.f);
	this->addChild(playerSprite, 1);
}

StageModePlayer::~StageModePlayer()
{
	if (!m_cSelectedGrids.empty())
	{
		m_cSelectedGrids.clear();
		m_cSelectedGrids.~Vector();
	}
}

void StageModePlayer::setName(const std::string s)
{
	this->m_sPlayerName = s;
}

std::string StageModePlayer::getName()
{
	return this->m_sPlayerName;
}

void StageModePlayer::setPosition(Vec2 pos)
{
	auto playerSprite = this->getChildByName("Player Sprite");
	if (playerSprite)
	{
		playerSprite->setPosition(pos);
		this->m_v2PlayerPos = pos;
	}
	else
	{
		MessageBox("Player Sprite does not exist!", "ERROR");
	}
}

Vec2 StageModePlayer::getPosition()
{
	return this->m_v2PlayerPos;
}

void StageModePlayer::setIndex(Vec2 index)
{
	this->m_v2PlayerIndex = index;
}

Vec2 StageModePlayer::getIndex()
{
	return this->m_v2PlayerIndex;
}

void StageModePlayer::setPiecePosition(GameBoard::PIECE_POSITION pp)
{
	this->m_ePlayerPP = pp;
}

GameBoard::PIECE_POSITION StageModePlayer::getPiecePosition()
{
	return this->m_ePlayerPP;
}

void StageModePlayer::setFirstMove(bool f)
{
	this->m_bFirstMove = f;
}

bool StageModePlayer::getFirstMove()
{
	return this->m_bFirstMove;
}

void StageModePlayer::setScore(int s)
{
	this->m_iPlayerScore = s;
}

int StageModePlayer::getScore()
{
	return this->m_iPlayerScore;
}

void StageModePlayer::incrementScoreBy(int i)
{
	this->m_iPlayerScore += i;
}

void StageModePlayer::setLinesMade(int l)
{
	this->m_iLinesMade = l;
}

int StageModePlayer::getLinesMade()
{
	return this->m_iLinesMade;
}

void StageModePlayer::setPreviousLines(int l)
{
	this->m_iPreviousLines = l;
}

int StageModePlayer::getPreviousLines()
{
	return this->m_iPreviousLines;
}

void StageModePlayer::setOldGrid(Grid * o)
{
	this->m_cOldGrid = o;
}

Grid * StageModePlayer::getOldGrid()
{
	return this->m_cOldGrid;
}

void StageModePlayer::setOldIndex(Vec2 oi)
{
	this->m_v2OldIndex = oi;
}

Vec2 StageModePlayer::getOldIndex()
{
	return this->m_v2OldIndex;
}

Sprite * StageModePlayer::getSprite()
{
	auto sprite = dynamic_cast<Sprite*>(this->getChildByName("Player Sprite"));
	if (sprite)
	{
		return sprite;
	}
	return nullptr;
}

void StageModePlayer::toggleCursor()
{
	this->m_bCursorToggled = !m_bCursorToggled;
}

bool StageModePlayer::checkScoringTile(GameBoard * gameboard, Mission * mission) // Mission * mission
{
	if (this->m_cSelectedGrids.size() > 0)
	{
		// Variables
		int selectedGridIcon;
		// First Tile in Selected List
		Grid * grid1 = this->m_cSelectedGrids.at(0);
		if (grid1)
		{
			selectedGridIcon = grid1->getIconID();
			// Check the tile the player is standing on
			auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);

			// Matching Icons
			if (selectedGridIcon == boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getIconID())
			{
				if (mission->getMinimumRequired())
				{
					if (this->m_cSelectedGrids.size() + 1 < mission->getMinimumBoxes())
					{
						// Move Player Back To Initial Position
						this->Reset(gameboard);
						// Invalid Selection
						for (Grid* i : m_cSelectedGrids)
						{

							i->removeChildByName("Player Clone");
							i->setCloned(false);
						}
						m_cSelectedGrids.clear();
						return false;
					}
				}
				if (mission->checkMissionBoxes(this->m_cSelectedGrids.size() + 1))
				{
					// Save Current Position in case of reset
					this->m_cOldGrid = boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y);
					this->m_v2OldIndex = m_v2PlayerIndex;

					m_cSelectedGrids.pushBack(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y));
					boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->setCloned(true);
					boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->addChild(this->createClone(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition()), 1);

					// Add to player score
					this->m_iPlayerScore += mission->getScorePoints(this->m_cSelectedGrids.size());

					if (mission->getMissionType() == Mission::MISSION_CUSTOM)
					{
						if (mission->getFindIcon())
						{
							if (mission->getInLine())
							{
								bool IconInside = false;
								for (Grid * g : m_cSelectedGrids)
								{
									if (g->getIconID() == mission->getIconID())
									{
										IconInside = true;
										break;
									}
								}
								if (IconInside)
								{
									this->m_iPreviousLines = m_iLinesMade;
									this->m_iLinesMade++;
								}
								else
								{
									auto playerAvatar = gameboard->getParent()->getChildByName("Player Node")->getChildByName("Default Avatar");
									if (playerAvatar)
									{
										playerAvatar->setColor(Color3B::MAGENTA);
									}
									SimpleAudioEngine::sharedEngine()->playEffect("./FX/StageMode/hard3_pattern.wav");
									// Stun Player for 5s
									this->m_fStunDuration = 5.f;
								}
							}
							else
							{
								if (mission->getIconID() == boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getIconID())
								{
									this->m_iPreviousLines = m_iLinesMade;
									this->m_iLinesMade++;
								}
							}
						}
						else
						{
							this->m_iPreviousLines = m_iLinesMade;
							this->m_iLinesMade++;
						}
					}
					else
					{
						this->m_iPreviousLines = m_iLinesMade;
						this->m_iLinesMade++;
					}

					// Every grid in vector, addChild(darkenedTile)
					for (int counter = 0; counter < m_cSelectedGrids.size(); counter++)
					{
						Grid * i = m_cSelectedGrids.at(counter);
						i->addChild(Sprite::create("Icons/iconplaceholder.png"), 2);
						// ANIMATION
						MoveBy * move;
						Sequence * animSeq;
						Label * score;
						if (counter < mission->getBoxes())
						{
							score = Label::createWithTTF("+200", "fonts/arial.ttf", 24);
							score->enableOutline(Color4B(0, 0, 0, 255), 3);
						}
						else
						{
							score = Label::createWithTTF("-200", "fonts/arial.ttf", 24);
							score->enableOutline(Color4B(255, 0, 0, 255), 3);
						}
						score->setName("Score");
						score->setColor(Color3B(255, 255, 255));
						//score->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.9f)));
						i->addChild(score, 2);
						move = MoveBy::create(1.f, Vec2(0, 15.f));
						auto removeRest = CallFunc::create([this, score](){
							score->removeFromParent();
						});
						animSeq = Sequence::create(move, removeRest, nullptr);
						score->runAction(cocos2d::FadeOut::create(1.f));
						score->runAction(animSeq);
					}
					m_cSelectedGrids.clear();
					return true;
				}
				else
				{
					// Move Player Back To Initial Position
					this->Reset(gameboard);
					// Invalid Selection
					for (Grid* i : m_cSelectedGrids)
					{

						i->removeChildByName("Player Clone");
						i->setCloned(false);
					}
					m_cSelectedGrids.clear();
					return false;
				}
			}
			else
			{
				// Move Player Back To Initial Position
				this->Reset(gameboard);
				// Invalid Selection
				for (Grid* i : m_cSelectedGrids)
				{
					i->removeChildByName("Player Clone");
					i->setCloned(false);
				}
				m_cSelectedGrids.clear();
				return false;
			}
		}
	}
	return false;
}

void StageModePlayer::MoveUp(GameBoard * gameboard)
{
	// Check Y Index First
	if (this->m_v2PlayerIndex.y > 0) // Allow Movement (default)
	{
		this->m_ePreviousDirection = DIRECTION_UP;

		Vec2 oldPositionIndex = m_v2PlayerIndex;
		this->m_v2PlayerIndex.set(oldPositionIndex.x, oldPositionIndex.y - 1);

		// Move In Current Piece
		auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
		// Check if Grid is active
		if ((boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
		{
			if (!this->m_bFirstMove)
			{
				if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
				{
					this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
				}
			}
			this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
		}
		else
		{
			// No Movement
			this->m_v2PlayerIndex.set(oldPositionIndex);
		}
	}
	else // Check Piece Position
	{
		if (this->m_ePlayerPP == GameBoard::PP_TOP || this->m_ePlayerPP == GameBoard::PP_TOPLEFT || this->m_ePlayerPP == GameBoard::PP_TOPRIGHT)
		{
			return; // Deny Movement 
		}
		else
		{
			this->m_ePreviousDirection = DIRECTION_UP;
			// Get Player Position Index
			Vec2 oldPositionIndex = m_v2PlayerIndex;
			this->m_v2PlayerIndex.set(oldPositionIndex.x, 3);

			// Get Old Board Piece
			auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
			// Get Player
			this->retain();

			switch (this->m_ePlayerPP)
			{
			case GameBoard::PP_MIDDLE:
				// Switch Player Pieces
				this->m_ePlayerPP = GameBoard::PP_TOP;

				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_MIDDLE;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOM:
				this->m_ePlayerPP = GameBoard::PP_MIDDLE;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOM;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOMLEFT:
				this->m_ePlayerPP = GameBoard::PP_LEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOMLEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOMRIGHT:
				this->m_ePlayerPP = GameBoard::PP_RIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOMRIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_LEFT: // TO BE FIXED
				this->m_ePlayerPP = GameBoard::PP_TOPLEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_LEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_RIGHT:
				this->m_ePlayerPP = GameBoard::PP_TOPRIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_v2PlayerIndex.set(oldPositionIndex);
					this->m_ePlayerPP = GameBoard::PP_RIGHT;
				}
				break;
			}
			this->release();
		}
	}
}

void StageModePlayer::MoveDown(GameBoard * gameboard)
{
	// Check Y Index First
	if (this->m_v2PlayerIndex.y < 3) // Allow Movement (default)
	{
		this->m_ePreviousDirection = DIRECTION_DOWN;
		Vec2 oldPositionIndex = m_v2PlayerIndex;
		this->m_v2PlayerIndex.set(oldPositionIndex.x, oldPositionIndex.y + 1);

		// Move In Current Piece
		auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);

		// Check if Grid is active
		if ((boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
		{
			if (!this->m_bFirstMove)
			{
				if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
				{
					this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
				}
			}
			this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
		}
		else
		{
			// No Movement
			this->m_v2PlayerIndex.set(oldPositionIndex);
		}
	}
	else // Check Piece Position
	{
		if (this->m_ePlayerPP == GameBoard::PP_BOTTOM || this->m_ePlayerPP == GameBoard::PP_BOTTOMLEFT || this->m_ePlayerPP == GameBoard::PP_BOTTOMRIGHT)
		{
			return; // Deny Movement 
		}
		else
		{
			this->m_ePreviousDirection = DIRECTION_DOWN;
			// Get Player Position Index
			Vec2 oldPositionIndex = m_v2PlayerIndex;
			this->m_v2PlayerIndex.set(oldPositionIndex.x, 0);

			// Get Old Board Piece
			auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
			// Get Player
			this->retain();

			switch (this->m_ePlayerPP)
			{
			case GameBoard::PP_MIDDLE:
				// Switch Player Pieces
				this->m_ePlayerPP = GameBoard::PP_BOTTOM;

				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_MIDDLE;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOP:
				this->m_ePlayerPP = GameBoard::PP_MIDDLE;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOP;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOPLEFT:
				this->m_ePlayerPP = GameBoard::PP_LEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOPLEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOPRIGHT:
				this->m_ePlayerPP = GameBoard::PP_RIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOPRIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_LEFT:
				this->m_ePlayerPP = GameBoard::PP_BOTTOMLEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_LEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_RIGHT:
				this->m_ePlayerPP = GameBoard::PP_BOTTOMRIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_RIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			}
			this->release();
		}
	}
}

void StageModePlayer::MoveLeft(GameBoard * gameboard)
{
	// Check X Index First
	if (this->m_v2PlayerIndex.x > 0) // Allow Movement (default)
	{
		this->m_ePreviousDirection = DIRECTION_LEFT;
		Vec2 oldPositionIndex = m_v2PlayerIndex;
		this->m_v2PlayerIndex.set(oldPositionIndex.x - 1, oldPositionIndex.y);

		// Move In Current Piece
		auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);

		// Check if Grid is active
		if ((boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
		{
			if (!this->m_bFirstMove)
			{
				if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
				{
					this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
				}
			}
			this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
		}
		else
		{
			// No Movement
			this->m_v2PlayerIndex.set(oldPositionIndex);
		}
	}
	else // Check Piece Position
	{
		if (this->m_ePlayerPP == GameBoard::PP_LEFT || this->m_ePlayerPP == GameBoard::PP_TOPLEFT || this->m_ePlayerPP == GameBoard::PP_BOTTOMLEFT)
		{
			return; // Deny Movement 
		}
		else
		{
			this->m_ePreviousDirection = DIRECTION_LEFT;
			// Get Player Position Index
			Vec2 oldPositionIndex = m_v2PlayerIndex;
			this->m_v2PlayerIndex.set(3, oldPositionIndex.y);

			// Get Old Board Piece
			auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
			// Get Player
			this->retain();

			switch (this->m_ePlayerPP)
			{
			case GameBoard::PP_MIDDLE:
				// Switch Player Pieces
				this->m_ePlayerPP = GameBoard::PP_LEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_MIDDLE;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOM:
				this->m_ePlayerPP = GameBoard::PP_BOTTOMLEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOM;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOP:
				this->m_ePlayerPP = GameBoard::PP_TOPLEFT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOP;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOMRIGHT:
				this->m_ePlayerPP = GameBoard::PP_BOTTOM;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOMRIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_RIGHT:
				this->m_ePlayerPP = GameBoard::PP_MIDDLE;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_RIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOPRIGHT:
				this->m_ePlayerPP = GameBoard::PP_TOP;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOPRIGHT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			}
			this->release();
		}
	}
}

void StageModePlayer::MoveRight(GameBoard * gameboard)
{
	// Check X Index First
	if (this->m_v2PlayerIndex.x < 3) // Allow Movement (default)
	{
		this->m_ePreviousDirection = DIRECTION_RIGHT;
		Vec2 oldPositionIndex = m_v2PlayerIndex;
		this->m_v2PlayerIndex.set(oldPositionIndex.x + 1, oldPositionIndex.y);

		// Move In Current Piece
		auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);

		// Check if Grid is active
		if ((boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
		{
			if (!this->m_bFirstMove)
			{
				if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
				{
					this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
					boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
				}
			}
			this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
		}
		else
		{
			// No Movement
			this->m_v2PlayerIndex.set(oldPositionIndex);
		}
	}
	else // Check Piece Position
	{
		if (this->m_ePlayerPP == GameBoard::PP_RIGHT || this->m_ePlayerPP == GameBoard::PP_TOPRIGHT || this->m_ePlayerPP == GameBoard::PP_BOTTOMRIGHT)
		{
			return; // Deny Movement 
		}
		else
		{
			this->m_ePreviousDirection = DIRECTION_RIGHT;
			// Get Player Position Index
			Vec2 oldPositionIndex = m_v2PlayerIndex;
			this->m_v2PlayerIndex.set(0, oldPositionIndex.y);

			// Get Old Board Piece
			auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
			// Get Player
			this->retain();

			switch (this->m_ePlayerPP)
			{
			case GameBoard::PP_MIDDLE:
				// Switch Player Pieces
				this->m_ePlayerPP = GameBoard::PP_RIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_MIDDLE;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOM:
				this->m_ePlayerPP = GameBoard::PP_BOTTOMRIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOM;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOP:
				this->m_ePlayerPP = GameBoard::PP_TOPRIGHT;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOP;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_BOTTOMLEFT:
				this->m_ePlayerPP = GameBoard::PP_BOTTOM;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_BOTTOMLEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_LEFT:
				this->m_ePlayerPP = GameBoard::PP_MIDDLE;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_LEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			case GameBoard::PP_TOPLEFT:
				this->m_ePlayerPP = GameBoard::PP_TOP;
				// Check new BoardPiece grid is active
				if ((gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getActive()) && (!gameboard->getBoardPiece(this->m_ePlayerPP)->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getCloned()))
				{
					// Remove Player from Old Piece
					boardPiece->removeChild(this);

					if (!this->m_bFirstMove)
					{
						if (!boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getCloned())
						{
							this->m_cSelectedGrids.pushBack(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y));
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->setCloned(true);
							boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->addChild(this->createClone(boardPiece->getGrid(oldPositionIndex.x, oldPositionIndex.y)->getPosition()),1);
						}
					}
					// Set New Board Piece
					boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
					boardPiece->addChild(this, 1);
					this->setPosition(boardPiece->getGrid(m_v2PlayerIndex.x, m_v2PlayerIndex.y)->getPosition());
				}
				else
				{
					this->m_ePlayerPP = GameBoard::PP_TOPLEFT;
					this->m_v2PlayerIndex.set(oldPositionIndex);
				}
				break;
			}
			this->release();
		}
	}
}

void StageModePlayer::Reset(GameBoard * gameboard)
{
	auto oldboardPiece = dynamic_cast<BoardPiece*>(this->m_cOldGrid->getParent());
	if (oldboardPiece)
	{
		// Get Current Board Piece
		auto boardPiece = gameboard->getBoardPiece(this->m_ePlayerPP);
		// Get Player
		this->retain();
		// Remove Player from Old Piece
		boardPiece->removeChild(this);
		// Change Board Piece
		this->m_ePlayerPP = GameBoard::PIECE_POSITION(oldboardPiece->getPositionID());
		// Set Position
		oldboardPiece->addChild(this, 1);
		this->setPosition(oldboardPiece->getGrid(m_v2OldIndex.x, m_v2OldIndex.y)->getPosition());
		// Set Index
		this->m_v2PlayerIndex = m_v2OldIndex;
		// Allow Release
		this->release();
		// First Move Reset
		this->m_bFirstMove = true;
	}
	else
	{
		MessageBox("oldboardpiece is null (failed check), see Josh", "ERROR");
	}
}

Sprite * StageModePlayer::createClone(Vec2 pos)
{
	auto playerClone = Sprite::create("Cursors/PlayerCursorTint.png");
	playerClone->setName("Player Clone");
	playerClone->setScale(1.1f);
	return playerClone;
}

void StageModePlayer::printDebugInfo()
{
	std::stringstream ss;
	ss << "Player Name : " << m_sPlayerName
		<< "\n Player Index : x = " << m_v2PlayerIndex.x << " y = " << m_v2PlayerIndex.y
		<< "\n Player Position : " << m_v2PlayerPos.x << "," << m_v2PlayerPos.y
		<< "\n Player Piece : " << m_ePlayerPP
		<< "\n Cursor Toggled : " << std::boolalpha << m_bCursorToggled;
	MessageBox(ss.str().c_str(), "PLAYER INFORMATION");
}

void StageModePlayer::setAvatarID(int i)
{
	this->m_iAvatarID = i;
}

int StageModePlayer::getAvatarID()
{
	return this->m_iAvatarID;
}

void StageModePlayer::setPreviousDirection(PLAYER_DIRECTION dir)
{
	this->m_ePreviousDirection = dir;
}

StageModePlayer::PLAYER_DIRECTION StageModePlayer::getPreviousDirection()
{
	return this->m_ePreviousDirection;
}

void StageModePlayer::setStunDuration(float f)
{
	this->m_fStunDuration = f;
}

float StageModePlayer::getStunDuration()
{
	return this->m_fStunDuration;
}