#include "BoardPiece.h"

#include "Grid.h"

BoardPiece::BoardPiece(GameBoard::PIECE_POSITION pp, GameBoard::BOARD_TYPE bt)
: m_ePositionID(pp)
, m_iPopulationCount(0)
, m_fPieceSize(0.f)
, m_iTotalActive(0)
{
	this->setScale(GLOBAL_UI_SCALE);

	auto placeholder = Sprite::create("200x200Placeholder.png");
	placeholder->setName("Placeholder");
	placeholder->setScale(0.5f);
	this->m_fPieceSize = placeholder->getContentSize().height * GLOBAL_UI_SCALE;
	//this->addChild(placeholder,-1);

	float startX = -m_fPieceSize * 0.36;
	float startY = m_fPieceSize * 0.36;
	float incrementY = 0.f;

	for (unsigned int i = 0; i < 4; i++)
	{
		startX = -m_fPieceSize * 0.36;
		for (unsigned int j = 0; j < 4; j++)
		{
			m_cArea[i][j] = new Grid("Icons/iconplaceholder.png");
			m_cArea[i][j]->setScale(0.9f);
			m_cArea[i][j]->setPosition(Vec2(startX - (m_cArea[i][j]->getSprite()->getContentSize().width * 0.5f), startY + (m_cArea[i][j]->getSprite()->getContentSize().height * 0.5f)));
			incrementY = m_cArea[i][j]->getSprite()->getContentSize().height;
			this->addChild(m_cArea[i][j]);
			
			// FOR DEBUGGING ONLY
			/*std::stringstream ss;
			ss << "x" << j << "y" << i;
			auto test = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 12);
			test->setName("test");
			test->setColor(Color3B(255, 255, 255));
			test->enableOutline(Color4B(0, 0, 0, 255), 3);
			test->setPosition(Vec2(m_cArea[i][j]->getPosition().x, m_cArea[i][j]->getPosition().y));
			this->addChild(test, 4);*/

			startX += m_cArea[i][j]->getSprite()->getContentSize().width;
		}
		startY -= incrementY;
	}

	this->Init(bt);
}

BoardPiece::~BoardPiece()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (m_cArea[i][j])
			delete m_cArea[i][j];
		}
	}
}

void BoardPiece::Update()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_cArea[i][j]->getChanging())
			{
				m_cArea[i][j]->ChangeIcon();
			}
		}
	}
}

void BoardPiece::Init(GameBoard::BOARD_TYPE bt)
{
	this->removeSpecificPieces(bt);
	if (m_iTotalActive > 1)
	{
		do
		{
			// Some Variables
			bool breakloop = false;

			// Choose a random icon
			int icon = cocos2d::random(1, 8);
			std::stringstream ss;
			ss << "Icons/icon" << icon << ".png";

			// Search if there was already a Grid with this ID
			for (unsigned i = 0; i < 4; i++)
			{
				for (unsigned j = 0; j < 4; j++)
				{
					// Check if Grid is Available
					if (m_cArea[i][j]->getActive())
					{
						if (m_cArea[i][j]->getIconID() == icon)
						{
							// Existing Pair Found, Exit Loop
							breakloop = true;
							break;
						}
					}
				}
				if (breakloop) // Why Continue Searching?
				{
					break;
				}
			}
			if (breakloop)
			{
				continue; // Generate a new Icon
			}
			else
			{
				Grid * grid1; // Slot 1
				Grid * grid2; // Slot 2
				// Dirty Fix
				bool grid1selected = false;
				bool grid2selected = false;
				bool pairMade = false;

				do
				{
					int randX = cocos2d::random(0, 3); // 4x4
					int randY = cocos2d::random(0, 3);
					if (!grid1selected) // Select Grid 1 First
					{
						if (m_cArea[randY][randX]->getActive())
						{
							if (!m_cArea[randY][randX]->getOccupied())
							{
								grid1selected = true;
								grid1 = m_cArea[randY][randX];
							}
						}
					}
					else if (!grid2selected)
					{
						if (m_cArea[randY][randX]->getActive())
						{
							if (m_cArea[randY][randX] == grid1) // Don't select same grid
							{
								continue;
							}
							else
							{
								if (!m_cArea[randY][randX]->getOccupied())
								{
									grid2selected = true;
									grid2 = m_cArea[randY][randX];
								}
							}
						}
					}
					else
					{
						Grid * old1 = grid1;
						Grid * old2 = grid2;
						old1->getSprite()->removeFromParent();
						old2->getSprite()->removeFromParent();

						grid1->setIconID(icon);
						grid1->setOccupied(true);
						auto newSprite = Sprite::create(ss.str());
						newSprite->setName("Sprite");
						grid1->setSprite(newSprite);
						grid1->addChild(newSprite, -1);

						grid2->setIconID(icon);
						grid2->setOccupied(true);
						newSprite = Sprite::create(ss.str());
						newSprite->setName("Sprite");
						grid2->setSprite(newSprite);
						grid2->addChild(newSprite, -1);

						pairMade = true;
					}
				} while (!pairMade);
				m_iPopulationCount++;
			}
		} while (this->m_iPopulationCount < ((int)(this->m_iTotalActive * 0.5)));
	}
	// Check for any non assigned actives (for (No. % 2) == 1)
	cocos2d::Vector<Grid*> unassignedList;
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (m_cArea[i][j]->getActive())
			{
				if (m_cArea[i][j]->getIconID() == 0)
				{
					//MessageBox("Test", "Test");
					unassignedList.pushBack(m_cArea[i][j]);
				}
			}
		}
	}

	if (unassignedList.size() > 0)
	{
		do
		{
			// Choose a random icon
			int icon = cocos2d::random(1, 8);
			std::stringstream ss;
			ss << "Icons/icon" << icon << ".png";

			Grid * old1 = unassignedList.back();
			old1->getSprite()->removeFromParent();
			old1->setIconID(icon);
			old1->setOccupied(true);
			old1->setActive(true);
			auto newSprite = Sprite::create(ss.str());
			newSprite->setName("Sprite");
			old1->setSprite(newSprite);
			old1->addChild(newSprite, -1);

			unassignedList.popBack();
		} while (unassignedList.size() > 0);
	}
}

void BoardPiece::removeSpecificPieces(GameBoard::BOARD_TYPE bt)
{
	/* IMPORTANT NOTE - Grid[y][x] */

	switch (bt)
	{
	case GameBoard::TYPE_NORMAL:
		switch ((GameBoard::PIECE_POSITION)this->m_ePositionID)
		{
		case GameBoard::PP_MIDDLE:
			break;
		case GameBoard::PP_TOP:
			break;
		case GameBoard::PP_TOPLEFT:
			break;
		case GameBoard::PP_TOPRIGHT:
			break;
		case GameBoard::PP_BOTTOM:
			break;
		case GameBoard::PP_BOTTOMLEFT:
			break;
		case GameBoard::PP_BOTTOMRIGHT:
			break;
		case GameBoard::PP_LEFT:
			break;
		case GameBoard::PP_RIGHT:
			break;
		}
		break;
	case GameBoard::TYPE_CIRCLE:
		switch ((GameBoard::PIECE_POSITION)this->m_ePositionID)
		{
		case GameBoard::PP_MIDDLE:
			break;
		case GameBoard::PP_TOP:
			break;
		case GameBoard::PP_TOPLEFT:
			m_cArea[0][0]->setActive(false);
			m_cArea[1][0]->setActive(false);
			m_cArea[2][0]->setActive(false);
			m_cArea[3][0]->setActive(false);
			m_cArea[0][1]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[0][2]->setActive(false);
			m_cArea[0][3]->setActive(false);
			break;
		case GameBoard::PP_TOPRIGHT:
			m_cArea[0][0]->setActive(false);
			m_cArea[0][1]->setActive(false);
			m_cArea[0][2]->setActive(false);
			m_cArea[0][3]->setActive(false);
			m_cArea[1][2]->setActive(false);
			m_cArea[1][3]->setActive(false);
			m_cArea[2][3]->setActive(false);
			m_cArea[3][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOM:
			break;
		case GameBoard::PP_BOTTOMLEFT:
			m_cArea[0][0]->setActive(false);
			m_cArea[1][0]->setActive(false);
			m_cArea[2][0]->setActive(false);
			m_cArea[3][0]->setActive(false);
			m_cArea[2][1]->setActive(false);
			m_cArea[3][1]->setActive(false);
			m_cArea[3][2]->setActive(false);
			m_cArea[3][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOMRIGHT:
			m_cArea[0][3]->setActive(false);
			m_cArea[1][3]->setActive(false);
			m_cArea[2][3]->setActive(false);
			m_cArea[3][3]->setActive(false);
			m_cArea[2][2]->setActive(false);
			m_cArea[3][2]->setActive(false);
			m_cArea[3][0]->setActive(false);
			m_cArea[3][1]->setActive(false);
			break;
		case GameBoard::PP_LEFT:
			break;
		case GameBoard::PP_RIGHT:
			break;
		}
		break;
	case GameBoard::TYPE_HEART:
		switch ((GameBoard::PIECE_POSITION)this->m_ePositionID)
		{
		case GameBoard::PP_MIDDLE:
			break;
		case GameBoard::PP_TOP:
			m_cArea[0][0]->setActive(false);
			m_cArea[0][1]->setActive(false);
			m_cArea[0][2]->setActive(false);
			m_cArea[0][3]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[1][2]->setActive(false);
			break;
		case GameBoard::PP_TOPLEFT:
			m_cArea[0][0]->setActive(false);
			m_cArea[0][1]->setActive(false);
			m_cArea[1][0]->setActive(false);
			break;
		case GameBoard::PP_TOPRIGHT:
			m_cArea[0][2]->setActive(false);
			m_cArea[0][3]->setActive(false);
			m_cArea[1][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOM:
			m_cArea[3][0]->setActive(false);
			m_cArea[3][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOMLEFT:
			this->setAllInactive();
			m_cArea[0][2]->setActive(true);
			m_cArea[0][3]->setActive(true);
			m_cArea[1][3]->setActive(true);
			break;
		case GameBoard::PP_BOTTOMRIGHT:
			this->setAllInactive();
			m_cArea[0][0]->setActive(true);
			m_cArea[0][1]->setActive(true);
			m_cArea[1][0]->setActive(true);
			break;
		case GameBoard::PP_LEFT:
			m_cArea[3][0]->setActive(false);
			break;
		case GameBoard::PP_RIGHT:
			m_cArea[3][3]->setActive(false);
			break;
		}
		break;
	case GameBoard::TYPE_ACORN:
		switch ((GameBoard::PIECE_POSITION)this->m_ePositionID)
		{
		case GameBoard::PP_MIDDLE:
			break;
		case GameBoard::PP_TOP:
			m_cArea[0][0]->setActive(false);
			m_cArea[1][0]->setActive(false);
			break;
		case GameBoard::PP_TOPLEFT:
			this->setAllInactive();
			m_cArea[3][3]->setActive(true);
			break;
		case GameBoard::PP_TOPRIGHT:
			m_cArea[0][1]->setActive(false);
			m_cArea[0][3]->setActive(false);
			m_cArea[2][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOM:
			m_cArea[3][2]->setActive(false);
			m_cArea[3][3]->setActive(false);
			m_cArea[2][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOMLEFT:
			break;
		case GameBoard::PP_BOTTOMRIGHT:
			this->setAllInactive();
			m_cArea[0][0]->setActive(true);
			break;
		case GameBoard::PP_LEFT:
			m_cArea[0][0]->setActive(false);
			m_cArea[0][1]->setActive(false);
			m_cArea[1][0]->setActive(false);
			break;
		case GameBoard::PP_RIGHT:
			m_cArea[3][2]->setActive(false);
			m_cArea[3][3]->setActive(false);
			break;
		}
		break;
	case GameBoard::TYPE_CLOVER:
		switch ((GameBoard::PIECE_POSITION)this->m_ePositionID)
		{
		case GameBoard::PP_MIDDLE:
			break;
		case GameBoard::PP_TOP:
			m_cArea[1][0]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[1][2]->setActive(false);
			m_cArea[1][3]->setActive(false);
			m_cArea[2][1]->setActive(false);
			m_cArea[2][2]->setActive(false);
			m_cArea[2][3]->setActive(false);
			m_cArea[3][1]->setActive(false);
			break;
		case GameBoard::PP_TOPLEFT:
			m_cArea[0][0]->setActive(false);
			break;
		case GameBoard::PP_TOPRIGHT:
			m_cArea[0][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOM:
			m_cArea[0][2]->setActive(false);
			m_cArea[1][0]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[1][2]->setActive(false);
			m_cArea[2][0]->setActive(false);
			m_cArea[2][1]->setActive(false);
			m_cArea[2][2]->setActive(false);
			m_cArea[2][3]->setActive(false);
			break;
		case GameBoard::PP_BOTTOMLEFT:
			m_cArea[3][0]->setActive(false);
			break;
		case GameBoard::PP_BOTTOMRIGHT:
			m_cArea[3][3]->setActive(false);
			break;
		case GameBoard::PP_LEFT:
			m_cArea[0][1]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[2][1]->setActive(false);
			m_cArea[3][1]->setActive(false);
			m_cArea[1][2]->setActive(false);
			m_cArea[2][2]->setActive(false);
			m_cArea[2][3]->setActive(false);
			break;
		case GameBoard::PP_RIGHT:
			m_cArea[0][2]->setActive(false);
			m_cArea[1][2]->setActive(false);
			m_cArea[2][2]->setActive(false);
			m_cArea[3][2]->setActive(false);
			m_cArea[1][1]->setActive(false);
			m_cArea[2][1]->setActive(false);
			m_cArea[1][0]->setActive(false);
			break;
		}
		break;
	}
	
	// Count number of active grids
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			if (m_cArea[i][j]->getActive())
			{
				m_iTotalActive++;
			}
		}
	}
}

void BoardPiece::setAllInactive()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			this->m_cArea[i][j]->setActive(false);
			if (this->m_cArea[i][j]->getOccupied())
			{
				this->m_cArea[i][j]->getSprite()->removeFromParent();
				this->m_cArea[i][j]->setOccupied(false);
				this->m_cArea[i][j]->setIconID(0);
			}
		}
	}
	this->m_iTotalActive = 0;
}

void BoardPiece::setAllActive()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (!this->m_cArea[i][j]->getActive())
			{
				this->m_cArea[i][j]->setActive(true);
				this->m_iTotalActive++;
			}
		}
	}
}

void BoardPiece::createPairAtLocation(Vec2 Loc1, Vec2 Loc2)
{
	int icon = cocos2d::random(1, 8);
	std::stringstream ss;
	ss << "Icons/icon" << icon << ".png";

	m_cArea[(int)Loc1.y][(int)Loc1.x]->Reset();
	m_cArea[(int)Loc2.y][(int)Loc2.x]->Reset();

	Grid * grid1 = m_cArea[(int)Loc1.y][(int)Loc1.x];
	Grid * grid2 = m_cArea[(int)Loc2.y][(int)Loc2.x];

	grid1->setIconID(icon);
	grid1->setOccupied(true);
	auto newSprite = Sprite::create(ss.str());
	newSprite->setName("Sprite");
	grid1->setSprite(newSprite);
	grid1->addChild(newSprite, -1);

	grid2->setIconID(icon);
	grid2->setOccupied(true);
	newSprite = Sprite::create(ss.str());
	newSprite->setName("Sprite");
	grid2->setSprite(newSprite);
	grid2->addChild(newSprite, -1);
}

void BoardPiece::Reset(GameBoard::BOARD_TYPE bt)
{
	this->m_iPopulationCount = 0;
	this->m_iTotalActive = 0;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			m_cArea[i][j]->FakeReset();
		}
	}

	this->Init(bt);
}