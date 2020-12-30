#include "StageModeNPC.h"
#include "AIComponent.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

StageModeNPC::StageModeNPC(int id)
: m_cAIComponent(nullptr)
, m_sNPCText("")
, m_bScoreLines(false)
{
	switch (NPC(id))
	{
	case NPC_BISON:
		this->m_sName = "Bison";
		// Score
		this->m_iScoreList.push_back(2000);
		this->m_iScoreList.push_back(3000);
		this->m_iScoreList.push_back(4000);
		break;
	case NPC_TOSCAYA:
		this->m_sName = "Toscaya";
		// Score
		this->m_iScoreList.push_back(2000);
		this->m_iScoreList.push_back(3000);
		this->m_iScoreList.push_back(4000);
		break;
	case NPC_CEILTA:
		this->m_sName = "C-eilta";
		// Score
		this->m_iScoreList.push_back(2000);
		this->m_iScoreList.push_back(3000);
		this->m_iScoreList.push_back(4000);
		break;
	case NPC_HAJIME:
		this->m_sName = "Hajime";
		this->m_sNPCText = "Can you help me find a picture?";
		// Score
		this->m_iScoreList.push_back(3);
		this->m_iScoreList.push_back(4);
		this->m_iScoreList.push_back(5);
		this->m_bScoreLines = true;
		break;
	case NPC_POKA:
		this->m_sName = "Poka";
		this->m_sNPCText = "Where am I?";
		// Score
		this->m_iScoreList.push_back(5000);
		this->m_iScoreList.push_back(9000);
		this->m_iScoreList.push_back(12000);
		break;
	case NPC_TSUBASA:
		this->m_sName = "Tsubasa";
		this->m_sNPCText = "Enjoy the Fireworks!";
		// Score
		this->m_iScoreList.push_back(3000);
		this->m_iScoreList.push_back(4000);
		this->m_iScoreList.push_back(5000);
		break;
	case NPC_CHINGCHING:
		this->m_sName = "Ching Ching";
		this->m_sNPCText = "Yay Yay!";
		// Score
		this->m_iScoreList.push_back(5);
		this->m_iScoreList.push_back(7);
		this->m_iScoreList.push_back(9);
		this->m_bScoreLines = true;
		break;
	case NPC_ASELLIA:
		this->m_sName = "Gywvita Asellia";
		this->m_sNPCText = "Greetings, Stranger...";
		// Score
		this->m_iScoreList.push_back(3);
		this->m_iScoreList.push_back(5);
		this->m_iScoreList.push_back(6);
		this->m_bScoreLines = true;
		break;
	case NPC_YORSHTA:
		this->m_sName = "Gywvita Yorshta";
		this->m_sNPCText = "W-Who are you...?";
		// Score
		this->m_iScoreList.push_back(6000);
		this->m_iScoreList.push_back(9000);
		this->m_iScoreList.push_back(15000);
		break;
	case NPC_ABYSSASELLIA:
		this->m_sName = "Asellia in the Abyss";
		this->m_sNPCText = "...";
		// Score
		this->m_iScoreList.push_back(5000);
		this->m_iScoreList.push_back(8000);
		this->m_iScoreList.push_back(10000);
		break;
	}
	this->m_iID = (NPC)id;
	m_cAIComponent = new AIComponent(NPC(id));
}

StageModeNPC::~StageModeNPC()
{
	if (m_cAIComponent)
	{
		delete m_cAIComponent;
	}
}

void StageModeNPC::runPattern(GameBoard * gameboard, StageModePlayer * player, float time)
{
	if (m_cAIComponent)
	{
		switch (m_cAIComponent->getPattern())
		{
		case AIComponent::PATTERN_SPAWN_OBSTACLE_RANDOM:
			if (m_cAIComponent->getObstacleSpawnTime() > 0)
			{
				m_cAIComponent->setObstacleSpawnTimer(m_cAIComponent->getObstacleSpawnTimer() + time);
				if (m_cAIComponent->getObstacleSpawnTimer() >= m_cAIComponent->getObstacleSpawnTime())
				{
					m_cAIComponent->setObstacleSpawnTimer(0.f);
					bool selected = false;
					do
					{
						// Get a random position
						int randX = cocos2d::random(0, 3); // 4x4
						int randY = cocos2d::random(0, 3);
						int randomPiece = cocos2d::random(0, 8);
						if ((gameboard->getBoardPiece(randomPiece)->getGrid(randX, randY)->getCloned()) || ((player->getPiecePosition() == GameBoard::PIECE_POSITION(randomPiece)) && player->getPosition() == (Vec2(randX, randY))))
						{
							continue;
						}
						selected = true;
						gameboard->getBoardPiece(randomPiece)->getGrid(randX, randY)->setCloned(true);
						gameboard->getBoardPiece(randomPiece)->getGrid(randX, randY)->addChild(this->spawnObstacle(), 1);
					} while (!selected);
				}
			}
			else
			{
				MessageBox("getObstacleSpawnTime is 0 : divideByZero", "ERROR");
			}
			break;
		case AIComponent::PATTERN_SPAWN_OBSTACLE_LINE:
			if (m_cAIComponent->getSpawnAround())
			{
				if (player->getLinesMade() > player->getPreviousLines())
				{
					player->setPreviousLines(player->getLinesMade());
					int count = 0;
					do
					{
						BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());
						int randDirection = cocos2d::random(0, 7); // Random Direction
						Vec2 gridIndex = gameboard->getGridIndex(player->getIndex(), randDirection);
						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(player->getPiecePosition(), player->getIndex(), randDirection);
							boardPiece = gameboard->getBoardPieceByDirection(player->getPiecePosition(), randDirection, player->getIndex());
							if ((updatedGridIndex == Vec2(-1, -1)) || (boardPiece == nullptr))
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								continue;
							}
							boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->setCloned(true);
							boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->addChild(this->spawnObstacle(), 1);
							count++;
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								continue;
							}
							boardPiece->getGrid(gridIndex.x, gridIndex.y)->setCloned(true);
							boardPiece->getGrid(gridIndex.x, gridIndex.y)->addChild(this->spawnObstacle(), 1);
							count++;
						}
					} while (count < m_cAIComponent->getObstaclesToSpawn());
				}
			}
			else if (m_cAIComponent->getSpawnDiagonal()) // Diagonal is clockwise (top left, top right, bottom right, bottom left)
			{
				if (player->getPreviousLines() < player->getLinesMade())
				{
					player->setPreviousLines(player->getLinesMade());
					int count = 0;
					do
					{
						BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());

						// 0 2 7 5 (TopLeft,TopRight,BottomRight,BottomLeft
						int direction = -1;
						if (count == 0)
							direction = 0;
						if (count == 1)
							direction = 2;
						if (count == 2)
							direction = 7;
						if (count == 3)
							direction = 5;

						Vec2 gridIndex = gameboard->getGridIndex(player->getIndex(), direction);
						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(player->getPiecePosition(), player->getIndex(), direction);
							boardPiece = gameboard->getBoardPieceByDirection(player->getPiecePosition(), direction, player->getIndex());
							if (boardPiece == nullptr)
							{
								count++;
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								count++;
								continue;
							}
							boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->setCloned(true);
							boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->addChild(this->spawnObstacle(), 1);
							count++;
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								count++;
								continue;
							}
							boardPiece->getGrid(gridIndex.x, gridIndex.y)->setCloned(true);
							boardPiece->getGrid(gridIndex.x, gridIndex.y)->addChild(this->spawnObstacle(), 1);
							count++;
						}
					} while (count < m_cAIComponent->getObstaclesToSpawn());
				}
			}
			break;
		case AIComponent::PATTERN_FIND_SPECIFICICON:
			if (player->getPreviousLines() < player->getLinesMade())
			{
				player->setPreviousLines(player->getLinesMade());
				this->m_cAIComponent->generateNewIconID();
			}
			break;
		case AIComponent::PATTERN_RESETMAP:
			if (this->m_cAIComponent->getTimeElapsed() >= this->m_cAIComponent->getResetTime())
			{
				this->m_cAIComponent->setTimeElapsed(0.f);
				// Reset Map
				this->updateEvent();
				gameboard->Reset();
			}
			break;
		case AIComponent::PATTERN_DESTROYICON:
			if (this->m_cAIComponent->getTimeElapsed() >= this->m_cAIComponent->getDestroyTime()) // Destroy Time
			{
				this->m_cAIComponent->setTimeElapsed(0.f);
				// Increase Timer
				if (this->m_cAIComponent->getDestroyTime() < 60)
				{
					this->m_cAIComponent->setDestroyTime(this->m_cAIComponent->getDestroyTime() * 2);
				}
				// Destroy Icon
				do
				{
					if (m_cAIComponent->getDestroyedList().size() == 8)
						break;

					int iconToDestroy = cocos2d::random(1, 8);
					if (!this->m_cAIComponent->findDestroyed(iconToDestroy))
					{
						this->updateEvent();
						this->m_cAIComponent->addToDestroyedList(iconToDestroy);
						for (int i = 0; i < 8; i++)
						{
							BoardPiece * boardpiece = gameboard->getBoardPiece(i);
							for (int j = 0; j < 4; j++)
							{
								for (int k = 0; k < 4; k++)
								{
									if (boardpiece->getGrid(j, k)->getIconID() == iconToDestroy)
									{
										boardpiece->getGrid(j, k)->Destroy();
									}
								}
							}
						}
						break;
					}
				} while (true);
			}
			break;
		case AIComponent::PATTERN_NATURE:
			if (player->getPreviousLines() < player->getLinesMade())
			{
				player->setPreviousLines(player->getLinesMade());
				this->updateEvent();
				int skipDirection;
				switch (player->getPreviousDirection())
				{
				case StageModePlayer::DIRECTION_UP:
					skipDirection = 1;
					break;
				case StageModePlayer::DIRECTION_DOWN:
					skipDirection = 6;
					break;
				case StageModePlayer::DIRECTION_LEFT:
					skipDirection = 3;
					break;
				case StageModePlayer::DIRECTION_RIGHT:
					skipDirection = 4;
					break;
				}
				for (int i = 0; i < 8; i++)
				{
					if (i == skipDirection)
						continue;

					BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());
					Vec2 gridIndex = gameboard->getGridIndex(player->getIndex(), i);
					if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
					{
						Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(player->getPiecePosition(), player->getIndex(), i);
						boardPiece = gameboard->getBoardPieceByDirection(player->getPiecePosition(), i, player->getIndex());
						if (boardPiece == nullptr)
						{
							continue;
						}
						if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
						{
							continue;
						}
						boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->setCloned(true);
						boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->addChild(this->spawnObstacle(), 1);
					}
					else
					{
						if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
						{
							continue;
						}
						boardPiece->getGrid(gridIndex.x, gridIndex.y)->setCloned(true);
						boardPiece->getGrid(gridIndex.x, gridIndex.y)->addChild(this->spawnObstacle(), 1);
					}
				}
			}
			break;
		case AIComponent::PATTERN_CHAIN:
			if (player->getPreviousLines() < player->getLinesMade())
			{
				player->setPreviousLines(player->getLinesMade());
				gameboard->Reset();
				gameboard->createObstacles();
				// Variables Needed
				bool loop = true;
				int directions[4] = { 1, 3, 4, 6 };
				int directions2[4] = { 0, 2, 5, 7 };
				int directionIndex = 0;
				// Go Straight in 4 Directions (UP - 1 DOWN - 6 LEFT - 3 RIGHT - 4)
				do
				{
					Vec2 currentGridIndex = player->getIndex();
					GameBoard::PIECE_POSITION currentPiecePosition = player->getPiecePosition();
					BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());
					loop = true;
					do
					{
						Vec2 gridIndex = gameboard->getGridIndex(currentGridIndex, directions[directionIndex]);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, directions[directionIndex]);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, directions[directionIndex], currentGridIndex);
							if (boardPiece == nullptr)
							{
								loop = false;
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
							currentGridIndex = updatedGridIndex;
							currentPiecePosition = boardPiece->getPositionID();
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
							currentGridIndex = gridIndex;
						}
					} while (loop);
					directionIndex++;
				} while (directionIndex < 4);

				for (int i = 0; i < 4; i++)
				{
					Vec2 currentGridIndex = player->getIndex();
					GameBoard::PIECE_POSITION currentPiecePosition = player->getPiecePosition();
					BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());

					Vec2 gridIndex = gameboard->getGridIndex(currentGridIndex, directions2[i]);
					boardPiece = gameboard->getBoardPiece(currentPiecePosition);

					if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
					{
						Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, directions2[i]);
						boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, directions2[i], currentGridIndex);
						if (boardPiece == nullptr)
						{
							continue;
						}
						if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
						{
							Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
							// Unclone it
							newGrid->setCloned(false);
							newGrid->removeAllChildren();
							std::stringstream ss;
							ss << "Icons/icon" << newGrid->getIconID() << ".png";
							auto newSprite = Sprite::create(ss.str());
							newSprite->setName("Sprite");
							newGrid->setSprite(newSprite);
							newGrid->addChild(newSprite, -1);
						}
						currentGridIndex = updatedGridIndex;
						currentPiecePosition = boardPiece->getPositionID();
					}
					else
					{
						if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
						{
							Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
							// Unclone it
							newGrid->setCloned(false);
							newGrid->removeAllChildren();
							std::stringstream ss;
							ss << "Icons/icon" << newGrid->getIconID() << ".png";
							auto newSprite = Sprite::create(ss.str());
							newSprite->setName("Sprite");
							newGrid->setSprite(newSprite);
							newGrid->addChild(newSprite, -1);
						}
						currentGridIndex = gridIndex;
					}

					switch (directions2[i])
					{
					case 0: // Top Left
						// Top
						gridIndex = gameboard->getGridIndex(currentGridIndex, 1);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 1);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 1, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}

						// Left
						gridIndex = gameboard->getGridIndex(currentGridIndex, 3);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 3);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 3, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						break;
					case 2: // Top Right
						// Top
						gridIndex = gameboard->getGridIndex(currentGridIndex, 1);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 1);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 1, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}

						// Right
						gridIndex = gameboard->getGridIndex(currentGridIndex, 4);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 4);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 4, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						break;
					case 5: // Bottom Left
						// Bottom
						gridIndex = gameboard->getGridIndex(currentGridIndex, 6);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 6);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 6, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}

						// Left
						gridIndex = gameboard->getGridIndex(currentGridIndex, 3);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 3);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 3, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						break;
					case 7: // Bottom Right
						// Bottom
						gridIndex = gameboard->getGridIndex(currentGridIndex, 6);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 6);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 6, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}

						// Right
						gridIndex = gameboard->getGridIndex(currentGridIndex, 4);
						boardPiece = gameboard->getBoardPiece(currentPiecePosition);

						if (((gridIndex.x < 0) || (gridIndex.x > 3)) || ((gridIndex.y < 0) || (gridIndex.y > 3)))
						{
							Vec2 updatedGridIndex = gameboard->getUpdatedGridIndex(currentPiecePosition, currentGridIndex, 4);
							boardPiece = gameboard->getBoardPieceByDirection(currentPiecePosition, 4, currentGridIndex);
							if (boardPiece == nullptr)
							{
								continue;
							}
							if (boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(updatedGridIndex.x, updatedGridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						else
						{
							if (boardPiece->getGrid(gridIndex.x, gridIndex.y)->getCloned())
							{
								Grid * newGrid = boardPiece->getGrid(gridIndex.x, gridIndex.y);
								// Unclone it
								newGrid->setCloned(false);
								newGrid->removeAllChildren();
								std::stringstream ss;
								ss << "Icons/icon" << newGrid->getIconID() << ".png";
								auto newSprite = Sprite::create(ss.str());
								newSprite->setName("Sprite");
								newGrid->setSprite(newSprite);
								newGrid->addChild(newSprite, -1);
							}
						}
						break;
					}
				}


				// Unclone the grid the player is standing on
				BoardPiece * boardPiece = gameboard->getBoardPiece(player->getPiecePosition());
				Grid * newGrid = boardPiece->getGrid(player->getIndex().x, player->getIndex().y);
				// Unclone it
				newGrid->setCloned(false);
				newGrid->removeAllChildren();
				std::stringstream ss;
				ss << "Icons/icon" << newGrid->getIconID() << ".png";
				auto newSprite = Sprite::create(ss.str());
				newSprite->setName("Sprite");
				newGrid->setSprite(newSprite);
				newGrid->addChild(newSprite, -1);
			}
			if (this->m_cAIComponent->getTimeElapsed() >= this->m_cAIComponent->getResetTime())
			{
				SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/break_event.wav", false);
				this->m_cAIComponent->setTimeElapsed(0.f);
				// Reset Map
				this->updateEvent();
				gameboard->Reset();
			}
			break;
		case AIComponent::PATTERN_CHANGINGTILES:
			m_cAIComponent->setObstacleSpawnTimer(m_cAIComponent->getObstacleSpawnTimer() + time);
			if (m_cAIComponent->getObstacleSpawnTimer() >= 0.5f)
			{
				m_cAIComponent->setObstacleSpawnTimer(0.f);
				gameboard->Update();
			}
			if (this->m_cAIComponent->getTimeElapsed() >= 5.f)
			{
				this->m_cAIComponent->setTimeElapsed(0.f);
				int counter = 0;
				do
				{
					int randY = cocos2d::random(0, 3);
					int randX = cocos2d::random(0, 3);
					int randBoardPiece = cocos2d::random(0, 8);
					if (gameboard->getBoardPiece(randBoardPiece)->getGrid(randX, randY)->getChanging())
					{
						continue;
					}
					if (gameboard->getBoardPiece(randBoardPiece)->getGrid(randX, randY)->getActive())
					{
						gameboard->getBoardPiece(randBoardPiece)->getGrid(randX, randY)->setChanging(true);
						counter++;
					}
				} while (counter < 4);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		MessageBox("AIComponent is null", "ERROR");
	}
}

void StageModeNPC::setID(int id)
{
	this->m_iID = (NPC)id;
}

int StageModeNPC::getID()
{
	return this->m_iID;
}

void StageModeNPC::setName(std::string s)
{
	this->m_sName = s;
}

std::string StageModeNPC::getName()
{
	return this->m_sName;
}

void StageModeNPC::setAIComponent(AIComponent * aic)
{
	this->m_cAIComponent = aic;
}

AIComponent * StageModeNPC::getAIComponent()
{
	return this->m_cAIComponent;
}

void StageModeNPC::setText(std::string s)
{
	this->m_sNPCText = s;
}

std::string StageModeNPC::getText()
{
	return this->m_sNPCText;
}

void StageModeNPC::updateEvent()
{
	switch (NPC(this->m_iID))
	{
	case NPC_BISON:
		this->m_sNPCText = "";
		break;
	case NPC_TOSCAYA:
		this->m_sNPCText = "";
		break;
	case NPC_CEILTA:
		this->m_sNPCText = "";
		break;
	case NPC_HAJIME:
		this->m_sNPCText = "";
		break;
	case NPC_POKA:
		this->m_sNPCText = "She is not here..?";
		break;
	case NPC_TSUBASA:
		this->m_sNPCText = "Boom! Boom!";
		break;
	case NPC_CHINGCHING:
		this->m_sNPCText = "";
		break;
	case NPC_ASELLIA:
		this->m_sNPCText = "The nature awakes...";
		break;
	case NPC_YORSHTA:
		this->m_sNPCText = "Go away!";
		break;
	case NPC_ABYSSASELLIA:
		this->m_sNPCText = "";
		break;
	}
}

Sprite * StageModeNPC::spawnObstacle()
{
	auto fadein = FadeIn::create(1.f);
	auto sprite = Sprite::create("Cursors/Obstacle.png");
	sprite->setName("Obstacle");
	sprite->setOpacity(0);
	sprite->setScale(0.95f);
	sprite->runAction(fadein);
	return sprite;
}

std::vector<int> StageModeNPC::getScoreList()
{
	return this->m_iScoreList;
}

void StageModeNPC::setScoreLines(bool b)
{
	this->m_bScoreLines = b;
}

bool StageModeNPC::getScoreLines()
{
	return this->m_bScoreLines;
}