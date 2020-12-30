#include "TutorialModeGame.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#include "TutorialModeMenu.h"
#include "StageModeMenu.h"

Scene* TutorialModeGame::createScene(int avatar1, int avatar2, int map, int level)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TutorialModeGame::create();

	// Create Game Board and Relevant Info
	layer->setStage(EASY_STAGE(level));
	layer->createBoard();
	layer->spawnPlayer(level);
	layer->spawnEnemy(avatar2);
	layer->createUI();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool TutorialModeGame::init()
{
	// Init Layer
	if (!Layer::init())
	{
		return false;
	}
	this->setName("Tutorial Mode Game"); // For Easier Referencing
	SimpleAudioEngine::getInstance()->stopAllEffects();
	// Gameplay Variables Init
	m_v2Player1PositionIndex.setZero();
	m_ePlayer1PP = GameBoard::PP_NONE;
	m_v2Player2PositionIndex.setZero();
	m_ePlayer2PP = GameBoard::PP_NONE;
	m_bKeyPressed = false;

	m_bShowGameOver = false;
	m_bReward1 = false;
	m_bReward2 = false;
	m_bReward3 = false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Timer
	this->m_cTimer = new StageModeTimer(60.f); // 1 Minute

	// Create Mouse and Keyboard Listeners
	this->createListeners();

	//Enable Update Overrides
	this->scheduleUpdate();

	return true;
}

void TutorialModeGame::update(float dt)
{
	auto tutorialUI = this->getChildByName("TutorialModeGame UI");
	if (tutorialUI)
	{
		auto line = dynamic_cast<Label*>(tutorialUI->getChildByName("Player Lines"));
		if (line)
		{
			std::stringstream ss2;
			ss2 << "Lines Made : " << m_cPlayer->getLinesMade();
			line->setString(ss2.str());
		}
		this->m_cTimer->update(dt,m_cNPC->getAIComponent());
		if (m_cNPC)
		{
			if (m_cTimer->getStart())
			{
				m_cNPC->runPattern(this->m_cGameBoard,this->m_cPlayer,dt);
			}
		}

		this->updateScore();

		if (!m_bShowGameOver)
		{
			if (this->m_cTimer->getCurrentTime() <= 0)
			{
				// Game Over
				this->m_cTimer->setStart(false);
				this->m_bShowGameOver = true;
				this->createGameOverMenu("Times Up!");
			}
			else if (this->m_cPlayer->getLinesMade() >= this->m_cMission->getLines())
			{
				// Line Limit Reached
				this->m_cTimer->setStart(false);
				this->m_bShowGameOver = true;
				this->createGameOverMenu("Stage Clear!");
			}
		}
	}
}

void TutorialModeGame::createBoard()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite * background;

	switch (this->m_eStage)
	{
	case EASY_1:
		background = Sprite::create("Background/easy.png");
		background->setName("Background");
		background->setPosition(Vec2(visibleSize.width * MIDDLE_WIDTH_OFFSET, visibleSize.height * MIDDLE_HEIGHT_OFFSET));
		this->addChild(background, -100);

		SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/easy_theme.wav", false);
		this->m_cGameBoard = new GameBoard();
		this->m_cGameBoard->setName("GameBoard");
		this->m_cGameBoard->setPositionY(-Director::getInstance()->getVisibleSize().height * GAMEBOARD_HEIGHT_OFFSET);

		// CUSTOM
		this->m_cMission = new Mission(5, 6);

		// Remove Everything
		// Create pair at location
		// Init rest
		this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->setAllInactive();
		this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->createPairAtLocation(Vec2(2, 3), Vec2(0, 0));
		this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->setAllActive();
		this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->Init(GameBoard::TYPE_NORMAL);
		this->addChild(this->m_cGameBoard, -1);
		break;
	case EASY_2:
		background = Sprite::create("Background/easy.png");
		background->setName("Background");
		background->setPosition(Vec2(visibleSize.width * MIDDLE_WIDTH_OFFSET, visibleSize.height * MIDDLE_HEIGHT_OFFSET));
		this->addChild(background, -100);

		SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/easy_theme.wav", false);
		this->m_cGameBoard = new GameBoard();
		this->m_cGameBoard->setName("GameBoard");
		this->m_cGameBoard->setPositionY(-Director::getInstance()->getVisibleSize().height * GAMEBOARD_HEIGHT_OFFSET);
		this->addChild(this->m_cGameBoard, -1);

		// CUSTOM
		this->m_cMission = new Mission(5, 6);
		
		// Create Inactive Tiles
		this->createInactiveTile(3, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);
		this->createInactiveTile(2, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);
		this->createInactiveTile(1, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);
		this->createInactiveTile(1, 2, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);
		this->createInactiveTile(1, 1, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);
		this->createInactiveTile(1, 0, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT,false);

		break;
	case EASY_3:
		background = Sprite::create("Background/easy_hidden.png");
		background->setName("Background");
		background->setPosition(Vec2(visibleSize.width * MIDDLE_WIDTH_OFFSET, visibleSize.height * MIDDLE_HEIGHT_OFFSET));
		this->addChild(background, -100);

		SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/hidden_theme.wav", false);
		this->m_cGameBoard = new GameBoard();
		this->m_cGameBoard->setName("GameBoard");
		this->m_cGameBoard->setPositionY(-Director::getInstance()->getVisibleSize().height * GAMEBOARD_HEIGHT_OFFSET);
		this->addChild(this->m_cGameBoard, -1);

		// CUSTOM
		this->m_cMission = new Mission(4, 6);
		
		this->createInactiveTile(3, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);
		this->createInactiveTile(2, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);
		this->createInactiveTile(1, 3, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);
		this->createInactiveTile(1, 2, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);
		this->createInactiveTile(1, 1, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);
		this->createInactiveTile(1, 0, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, false);

		this->createInactiveTile(0, 1, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, true);
		this->createInactiveTile(2, 1, GameBoard::PIECE_POSITION::PP_BOTTOMRIGHT, true);
		this->createInactiveTile(0, 3, GameBoard::PIECE_POSITION::PP_RIGHT, true);
		this->createInactiveTile(2, 3, GameBoard::PIECE_POSITION::PP_RIGHT, true);

		break;
	default:
		MessageBox("Tutorial Mode : Invalid m_eStage", "ERROR");
		this->menuCloseCallback(this);
		break;
	}
}

void TutorialModeGame::createInactiveTile(int x,int y,GameBoard::PIECE_POSITION pp, bool real)
{
	if (real)
	{
		auto fadein = FadeIn::create(1.f);
		auto sprite = Sprite::create("Cursors/Obstacle.png");
		sprite->setName("Obstacle");
		sprite->setOpacity(0);
		sprite->setScale(0.95f);
		sprite->runAction(fadein);
		this->m_cGameBoard->getBoardPiece(pp)->getGrid(x, y)->addChild(sprite, 1);
		this->m_cGameBoard->getBoardPiece(pp)->getGrid(x, y)->setCloned(true);
	}
	else
	{
		auto cursor = Sprite::create("Cursors/PlayerCursor.png");
		cursor->setName("Fake Cursor");
		cursor->setScale(1.11f);
		auto placeholder = Sprite::create("Icons/iconplaceholder.png");
		placeholder->setName("Fake Obstacle");
		this->m_cGameBoard->getBoardPiece(pp)->getGrid(x, y)->addChild(placeholder, 2);
		this->m_cGameBoard->getBoardPiece(pp)->getGrid(x, y)->addChild(cursor, 1);
		this->m_cGameBoard->getBoardPiece(pp)->getGrid(x, y)->setCloned(true);
	}
}

void TutorialModeGame::createUI()
{
	auto uiNode = Node::create();
	uiNode->setName("TutorialModeGame UI");

	/******* Game Information*******/
	// Timer
	uiNode->addChild(m_cTimer, 1);

	// Mission
	std::stringstream ss1;
	ss1 << "Mission : Create " << m_cMission->getLines() << " Lines using " << m_cMission->getBoxes() << " Boxes";
	auto mission = Label::createWithTTF(ss1.str(), "fonts/arial.ttf", 24);
	mission->setName("Mission Name");
	mission->setColor(Color3B(255, 255, 255));
	mission->enableOutline(Color4B(0, 0, 0, 255), 2);
	mission->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * MIDDLE_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * MISSION_HEIGHT_TEXT_OFFSET2)));
	uiNode->addChild(mission, 1);

	/********* Player UI *********/
	// Avatar
	auto avatarNode1 = Node::create();
	avatarNode1->setScale(GLOBAL_UI_SCALE);
	avatarNode1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * AVATAR_UI_HEIGHT_OFFSET)));
	avatarNode1->addChild(this->createAvatar(0), 1);
	this->addChild(avatarNode1, 1);

	// Name
	auto stageModeUI = Sprite::create("UI/name.png");
	stageModeUI->setName("Name UI");
	stageModeUI->setScale(GLOBAL_UI_SCALE);
	stageModeUI->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * NAME_UI_OFFSET)));

	auto name = Label::createWithTTF(this->m_cPlayer->getName(), "fonts/arial.ttf", 24);
	name->setName("Player Name");
	name->setColor(Color3B(255, 255, 255));
	name->enableOutline(Color4B(0, 0, 0, 255), 2);
	name->setPosition(Vec2(stageModeUI->getContentSize().width * MIDDLE_WIDTH_OFFSET, stageModeUI->getContentSize().height * MIDDLE_HEIGHT_OFFSET));
	stageModeUI->addChild(name, 1);
	uiNode->addChild(stageModeUI, 1);

	// Lines
	std::stringstream ss3;
	ss3 << "Lines Made : " << m_cPlayer->getLinesMade();
	auto line = Label::createWithTTF(ss3.str(), "fonts/arial.ttf", 24);
	line->setName("Player Lines");
	line->setColor(Color3B(255, 255, 255));
	line->enableOutline(Color4B(0, 0, 0, 255), 2);
	line->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * PLAYER_LINES_HEIGHT_OFFSET)));
	uiNode->addChild(line, 1);

	// Stars (Mission Rating)
	auto scoreBG = Sprite::create("UI/score.png");
	scoreBG->setName("Score UI");
	scoreBG->setScale(GLOBAL_UI_SCALE);
	scoreBG->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * MISSION_HEIGHT_OFFSET)));
	for (int i = 0; i < 3; i++)
	{
		auto star = Sprite::create("UI/blankstar.png");
		std::stringstream ss1;
		ss1 << i + 1;
		star->setName("Blank Star " + ss1.str());
		star->setScale(GLOBAL_UI_SCALE);
		star->setPosition(Vec2(scoreBG->getContentSize().width * (0.75f - (0.25f * i)), scoreBG->getContentSize().height * MIDDLE_HEIGHT_OFFSET));
		scoreBG->addChild(star, 1);
	}
	uiNode->addChild(scoreBG, 1);


	// Speech
	auto speechUI = Sprite::create("UI/rightbubble.png");
	speechUI->setName("Player Speech Bubble");
	speechUI->setScale(GLOBAL_UI_SCALE);
	speechUI->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * SPEECH_UI_HEIGHT_OFFSET)));

	auto speechText = Label::createWithTTF("...", "fonts/arial.ttf", 18);
	speechText->setAlignment(TextHAlignment::CENTER);
	speechText->setDimensions(240, 0);
	speechText->setName("Player Speech Text");
	speechText->setColor(Color3B(255, 255, 255));
	speechText->enableOutline(Color4B(0, 0, 0, 255), 2);
	speechText->setPosition(Vec2(speechUI->getContentSize().width * MIDDLE_WIDTH_OFFSET, speechUI->getContentSize().height * (MIDDLE_HEIGHT_OFFSET + SPEECH_TEXT_UI_HEIGHT_OFFSET)));
	speechUI->addChild(speechText, 1);
	uiNode->addChild(speechUI, 1);

	/********* NPC UI *********/
	//Avatar
	auto avatarNode2 = Node::create();
	avatarNode2->setScale(GLOBAL_UI_SCALE);
	avatarNode2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * ENEMY_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * AVATAR_UI_HEIGHT_OFFSET)));
	avatarNode2->addChild(this->createAvatar(this->m_cNPC->getID()), 1);
	this->addChild(avatarNode2, 1);

	// Name
	stageModeUI = Sprite::create("UI/name.png");
	stageModeUI->setName("Enemy Name UI");
	stageModeUI->setScale(GLOBAL_UI_SCALE);
	stageModeUI->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * ENEMY_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * NAME_UI_OFFSET)));

	name = Label::createWithTTF(this->m_cNPC->getName(), "fonts/arial.ttf", 24);
	name->setName("NPC Name");
	name->setColor(Color3B(255, 255, 255));
	name->enableOutline(Color4B(0, 0, 0, 255), 2);
	name->setPosition(Vec2(stageModeUI->getContentSize().width * MIDDLE_WIDTH_OFFSET, stageModeUI->getContentSize().height * MIDDLE_HEIGHT_OFFSET));
	stageModeUI->addChild(name, 1);
	uiNode->addChild(stageModeUI, 1);

	// Speech
	speechUI = Sprite::create("UI/leftbubble.png");
	speechUI->setName("NPC Speech Bubble");
	speechUI->setScale(GLOBAL_UI_SCALE);
	speechUI->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * ENEMY_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * SPEECH_UI_HEIGHT_OFFSET)));

	std::string s;
	switch (this->m_eStage)
	{
	case EASY_1:
		s = this->getStepText(m_eStep);
		break;
	case EASY_2:
		s = "Welcome to Stage 2!";
		break;
	case EASY_3:
		s = "Welcome to the Hidden Stage 3!";
		break;
	default:
		s = "...";
		break;
	}

	speechText = Label::createWithTTF(s, "fonts/arial.ttf", 18);
	speechText->setAlignment(TextHAlignment::CENTER);
	speechText->setDimensions(240, 0);
	speechText->setName("NPC Speech Text");
	speechText->setColor(Color3B(255, 255, 255));
	speechText->enableOutline(Color4B(0, 0, 0, 255), 2);
	speechText->setPosition(Vec2(speechUI->getContentSize().width * MIDDLE_WIDTH_OFFSET, speechUI->getContentSize().height * (MIDDLE_HEIGHT_OFFSET + SPEECH_TEXT_UI_HEIGHT_OFFSET)));
	speechUI->addChild(speechText, 1);
	uiNode->addChild(speechUI, 1);

	this->addChild(uiNode);
}

void TutorialModeGame::createGameOverMenu(std::string s)
{
	auto overlaySprite = Sprite::create("UI/ScoreboardUI.png");
	overlaySprite->setName("Gameover Overlay");
	overlaySprite->setScale(1.f);
	overlaySprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * MIDDLE_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * MIDDLE_HEIGHT_OFFSET)));
	
	auto speechText = Label::createWithTTF(s, "fonts/arial.ttf", 24);
	speechText->setColor(Color3B(255, 255, 255));
	speechText->enableOutline(Color4B(0, 0, 0, 255), 2);
	speechText->setPosition(Vec2(overlaySprite->getContentSize().width * MIDDLE_WIDTH_OFFSET, overlaySprite->getContentSize().height * 0.8f));
	overlaySprite->addChild(speechText, 1);

	for (int i = 0; i < 3; i++)
	{
		auto star = Sprite::create("UI/blankstar.png");
		std::stringstream ss1;
		ss1 << i + 1;
		star->setName("Blank Star " + ss1.str());
		star->setScale(GLOBAL_UI_SCALE);
		star->setPosition(Vec2(overlaySprite->getContentSize().width * (0.75f - (0.25f * i)), overlaySprite->getContentSize().height * 0.5f));
		overlaySprite->addChild(star, 1);
	}

	if (m_bReward3)
	{
		dynamic_cast<Sprite*>(overlaySprite->getChildByName("Blank Star 1"))->setTexture("UI/star.png");
	}
	if (m_bReward2)
	{
		dynamic_cast<Sprite*>(overlaySprite->getChildByName("Blank Star 2"))->setTexture("UI/star.png");
	}
	if (m_bReward1)
	{
		dynamic_cast<Sprite*>(overlaySprite->getChildByName("Blank Star 3"))->setTexture("UI/star.png");
	}
	
	auto continueText = Label::createWithTTF("Press Enter to continue", "fonts/arial.ttf", 18);
	continueText->setColor(Color3B(255, 255, 255));
	continueText->enableOutline(Color4B(0, 0, 0, 255), 2);
	continueText->setPosition(Vec2(overlaySprite->getContentSize().width * MIDDLE_WIDTH_OFFSET, overlaySprite->getContentSize().height * 0.2f));
	overlaySprite->addChild(continueText, 1);

	this->addChild(overlaySprite, 5);
}

Sprite * TutorialModeGame::createAvatar(int avatarID)
{
	std::stringstream dirCheck;
	dirCheck << "Avatars/avatar" << avatarID;

	if (FileUtils::getInstance()->isDirectoryExist(dirCheck.str()))
	{
		Sprite * avatar = nullptr;
		Vector<SpriteFrame*> animFrames;
		// Loop through folder to find animation frames
		bool loop = true;
		int fileCounter = 0;

		do
		{
			std::stringstream ss;
			ss << dirCheck.str() << "/frame_" << fileCounter << ".png";
			if (FileUtils::getInstance()->isFileExist(ss.str()))
			{
				if (fileCounter == 0) // First Frame Found
				{
					avatar = Sprite::create(ss.str()); // first 
					std::stringstream ss;
					ss << "Avatar " << avatarID;
					avatar->setName(ss.str());
				}
				animFrames.pushBack(SpriteFrame::create(ss.str(), Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
				fileCounter++;
			}
			else
			{
				loop = false;
			}

		} while (loop);

		if (fileCounter > 0)
		{
			// create the animation out of the frames
			Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			Animate* animate = Animate::create(animation);
			avatar->runAction(RepeatForever::create(animate));
		}
		else
		{
			// Placeholder Avatar
			avatar = Sprite::create("Avatars/default/default.png");
			avatar->setName("Default Avatar");
		}
		return avatar;
	}
	else
	{
		// Placeholder Avatar
		Sprite * avatar = Sprite::create("Avatars/default/default.png");
		avatar->setName("Default Avatar");
		return avatar;
	}
	return nullptr;
}

void TutorialModeGame::spawnPlayer(int level)
{
	// Create Player
	this->m_cPlayer = new StageModePlayer("Josh");

	if (m_cGameBoard)
	{
		switch (EASY_STAGE(level))
		{
		case EASY_1:
			m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 3)->getPosition());
			m_cPlayer->setIndex(Vec2(3, 3));
			m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
			m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 3));
			m_cPlayer->setOldIndex(Vec2(3, 3));
			this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
			break;
		case EASY_2:
			m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0)->getPosition());
			m_cPlayer->setIndex(Vec2(1, 0));
			m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
			m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0));
			m_cPlayer->setOldIndex(Vec2(1, 0));
			this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
			break;
		case EASY_3:
			m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0)->getPosition());
			m_cPlayer->setIndex(Vec2(1, 0));
			m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
			m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0));
			m_cPlayer->setOldIndex(Vec2(1, 0));
			this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
			break;
		default:
			break;
		}
	}
	else
	{
		MessageBox("Could not spawn player : GameBoard not created", "ERROR");
	}
}

void TutorialModeGame::spawnEnemy(int id)
{
	this->m_cNPC = new StageModeNPC(id);
	this->addChild(this->m_cNPC);
}

void TutorialModeGame::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void TutorialModeGame::createListeners()
{
	// Create Keyboard Listener
	auto kListener = EventListenerKeyboard::create();
	kListener->onKeyPressed = CC_CALLBACK_2(TutorialModeGame::onKeyPressed, this);
	kListener->onKeyReleased = CC_CALLBACK_2(TutorialModeGame::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kListener, this);

	// Create Mouse Listener
	auto mListener = EventListenerMouse::create();
	mListener->onMouseMove = CC_CALLBACK_1(TutorialModeGame::onMouseMove, this);
	mListener->onMouseUp = CC_CALLBACK_1(TutorialModeGame::onMouseUp, this);
	mListener->onMouseDown = CC_CALLBACK_1(TutorialModeGame::onMouseDown, this);
	mListener->onMouseScroll = CC_CALLBACK_1(TutorialModeGame::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);
}

void TutorialModeGame::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (!m_bKeyPressed)
	{
		m_bKeyPressed = true;

		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (!m_bShowGameOver)
			{
				if (m_cPlayer)
				{
					if (m_bInStep)
					{
						auto ui = this->getChildByName("TutorialModeGame UI");
						if (ui)
						{
							auto npcText = dynamic_cast<Label*>(ui->getChildByName("NPC Speech Bubble")->getChildByName("NPC Speech Text"));
							auto playerText = dynamic_cast<Label*>(ui->getChildByName("Player Speech Bubble")->getChildByName("Player Speech Text"));

							switch (m_eStep)
							{
							case STEP_1_10:
							case STEP_1_11:
								m_cPlayer->MoveUp(this->m_cGameBoard);
								if (m_cPlayer->getFirstMove())
								{
									m_cPlayer->setFirstMove(false);
								}
								m_eStep = EASY_STEPS(m_eStep + 1);
								break;
							case STEP_1_12:
								m_cPlayer->MoveUp(this->m_cGameBoard);
								if (m_cPlayer->getFirstMove())
								{
									m_cPlayer->setFirstMove(false);
								}
								m_eStep = EASY_STEPS(m_eStep + 1);
								playerText->removeAllChildren();
								break;
							default:
								break;
							}

							if (npcText)
							{
								npcText->setString(this->getStepText(m_eStep));
							}
						}
						else
						{
							MessageBox("TutorialModeGame UI Node not found", "ERROR!");
						}
					}
					else
					{
						m_cPlayer->MoveUp(this->m_cGameBoard);
						if (m_cPlayer->getFirstMove())
						{
							m_cPlayer->setFirstMove(false);
						}
					}
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (!m_bShowGameOver)
			{
				if (m_cPlayer)
				{
					if (m_bInStep)
					{
						auto ui = this->getChildByName("TutorialModeGame UI");
						if (ui)
						{
							auto npcText = dynamic_cast<Label*>(ui->getChildByName("NPC Speech Bubble")->getChildByName("NPC Speech Text"));
							auto playerText = dynamic_cast<Label*>(ui->getChildByName("Player Speech Bubble")->getChildByName("Player Speech Text"));

							switch (m_eStep)
							{
							case STEP_1_7:
								m_cPlayer->MoveLeft(this->m_cGameBoard);
								if (m_cPlayer->getFirstMove())
								{
									m_cPlayer->setFirstMove(false);
								}
								m_eStep = EASY_STEPS(m_eStep + 1);
								if (npcText)
								{
									npcText->setString(this->getStepText(m_eStep));

									this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(2, 3)->removeChildByName("Select Sprite");

									FadeIn * fadein = FadeIn::create(0.5f);
									FadeOut * fadeout = FadeOut::create(0.5f);
									Sequence * animSeq = Sequence::create(fadein, fadeout, nullptr);
									auto blankSprite = Sprite::create("Cursors/Blank.png");
									blankSprite->setName("Blank Sprite");
									blankSprite->setScale(0.9f);
									blankSprite->runAction(RepeatForever::create(animSeq));
									this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(0, 0)->addChild(blankSprite, 1);
								}
								break;
							case STEP_1_8:
							case STEP_1_9:
								m_cPlayer->MoveLeft(this->m_cGameBoard);
								if (m_cPlayer->getFirstMove())
								{
									m_cPlayer->setFirstMove(false);
								}
								m_eStep = EASY_STEPS(m_eStep + 1);
							default:
								break;
							}
						}
						else
						{
							MessageBox("TutorialModeGame UI Node not found", "ERROR!");
						}
					}
					else
					{
						m_cPlayer->MoveLeft(this->m_cGameBoard);
						if (m_cPlayer->getFirstMove())
						{
							m_cPlayer->setFirstMove(false);
						}
					}
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (!m_bShowGameOver)
			{
				if (m_cPlayer)
				{
					if (m_bInStep)
					{
						switch (m_eStep)
						{
						case STEP_1_7:
							break;
						}
					}
					else
					{
						m_cPlayer->MoveDown(this->m_cGameBoard);
						if (m_cPlayer->getFirstMove())
						{
							m_cPlayer->setFirstMove(false);
						}
					}
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (!m_bShowGameOver)
			{
				if (m_cPlayer)
				{
					if (m_bInStep)
					{
						switch (m_eStep)
						{
						case STEP_1_7:
							break;
						}
					}
					else
					{
						m_cPlayer->MoveRight(this->m_cGameBoard);
						if (m_cPlayer->getFirstMove())
						{
							m_cPlayer->setFirstMove(false);
						}
					}
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			if (m_bShowGameOver)
			{
				this->checkScore();
				Director::getInstance()->replaceScene(TransitionFade::create(1.f, StageModeMenu::createScene(), Color3B(0, 0, 0)));
			}
			else
			{
				if (m_cPlayer)
				{
					if (m_bInStep)
					{
						switch (m_eStep)
						{
						case STEP_1_7:
							if (m_cPlayer->getFirstMove())
							{
								return;
							}
							break;
						case STEP_1_8:
						case STEP_1_9:
						case STEP_1_10:
						case STEP_1_11:
							return;
							break;
						case STEP_1_13:
							if (m_cPlayer->checkScoringTile(this->m_cGameBoard, this->m_cMission))
							{
								// FirstMove = true
								if (!m_cPlayer->getFirstMove())
								{
									m_cPlayer->setFirstMove(true);
								}
							}
							m_eStep = EASY_STEPS(17);
							break;
						case STEP_1_21:
							this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(3, 0)->removeChildByName("Select Sprite 1");
							this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0)->removeChildByName("Select Sprite 2");
							this->m_cGameBoard->getBoardPiece(GameBoard::PP_RIGHT)->getGrid(0, 3)->removeChildByName("Select Sprite 3");
							break;
						}
						m_eStep = EASY_STEPS(m_eStep + 1);

						auto ui = this->getChildByName("TutorialModeGame UI");
						if (ui)
						{
							auto npcText = dynamic_cast<Label*>(ui->getChildByName("NPC Speech Bubble")->getChildByName("NPC Speech Text"));
							auto playerText = dynamic_cast<Label*>(ui->getChildByName("Player Speech Bubble")->getChildByName("Player Speech Text"));
							Sprite * sprite;
							FadeIn * fadein;
							FadeOut * fadeout;
							Sequence * animSeq;
							switch (m_eStep)
							{
							case STEP_1_7:
								sprite = Sprite::create("UI/up.png");
								sprite->setPosition(playerText->getContentSize().width * 0.5f, sprite->getContentSize().height * 0.75f);
								playerText->addChild(sprite, 1);
								sprite = Sprite::create("UI/down.png");
								sprite->setPosition(playerText->getContentSize().width * 0.5f, -sprite->getContentSize().height * 0.19f);
								playerText->addChild(sprite, 1);
								sprite = Sprite::create("UI/left.png");
								sprite->setPosition(playerText->getContentSize().width * 0.21f, -sprite->getContentSize().height * 0.19f);
								playerText->addChild(sprite, 1);
								sprite = Sprite::create("UI/right.png");
								sprite->setPosition(playerText->getContentSize().width * 0.79f, -sprite->getContentSize().height * 0.19f);
								playerText->addChild(sprite, 1);

								fadein = FadeIn::create(0.5f);
								fadeout = FadeOut::create(0.5f);
								animSeq = Sequence::create(fadein, fadeout, nullptr);
								sprite = Sprite::create("Cursors/SelectTile.png");
								sprite->setName("Select Sprite");
								sprite->runAction(RepeatForever::create(animSeq));
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(2, 3)->addChild(sprite, 1);
								break;
							case STEP_1_21:
								fadein = FadeIn::create(0.5f);
								fadeout = FadeOut::create(0.5f);
								animSeq = Sequence::create(fadein, fadeout, nullptr);

								sprite = Sprite::create("Cursors/SelectTile.png");
								sprite->setName("Select Sprite 1");
								sprite->runAction(RepeatForever::create(animSeq));
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(3, 0)->addChild(sprite, 1);

								fadein = FadeIn::create(0.5f);
								fadeout = FadeOut::create(0.5f);
								animSeq = Sequence::create(fadein, fadeout, nullptr);
								sprite = Sprite::create("Cursors/SelectTile.png");
								sprite->setName("Select Sprite 2");
								sprite->runAction(RepeatForever::create(animSeq));
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 0)->addChild(sprite, 1);

								fadein = FadeIn::create(0.5f);
								fadeout = FadeOut::create(0.5f);
								animSeq = Sequence::create(fadein, fadeout, nullptr);
								sprite = Sprite::create("Cursors/SelectTile.png");
								sprite->setName("Select Sprite 3");
								sprite->runAction(RepeatForever::create(animSeq));
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_RIGHT)->getGrid(0, 3)->addChild(sprite, 1);
								break;
							case STEP_1_26:
								fadein = FadeIn::create(1.f);
								sprite = Sprite::create("Cursors/Obstacle.png");
								sprite->setName("Obstacle 1");
								sprite->setOpacity(0);
								sprite->runAction(fadein);
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_MIDDLE)->getGrid(0, 0)->setCloned(true);
								this->m_cGameBoard->getBoardPiece(GameBoard::PP_MIDDLE)->getGrid(0, 0)->addChild(sprite, 1);
								break;
							case STEP_2_4:
								this->createInactiveTile(1, 3, GameBoard::PP_RIGHT, true);
								this->createInactiveTile(0, 3, GameBoard::PP_RIGHT, true);
								break;
							default:
								break;
							}

							if (npcText)
							{
								npcText->setString(this->getStepText(m_eStep));
							}
						}
						else
						{
							MessageBox("TutorialModeGame UI Node not found", "ERROR!");
						}

						switch (m_eStage)
						{
						case EASY_1:
							if (m_eStep >= STEP_1_TOTAL)
							{
								m_bInStep = false;
								m_cTimer->start();
							}
							break;
						case EASY_2:
							if (m_eStep >= STEP_2_TOTAL)
							{
								m_bInStep = false;
								m_cTimer->start();
							}
							break;
						case EASY_3:
							if (m_eStep >= STEP_3_TOTAL)
							{
								m_bInStep = false;
								m_cTimer->start();
							}
							break;
						default:
							break;
						}
					}
					else
					{
						// Score System (Check if valid tiles)
						if (m_cPlayer->checkScoringTile(this->m_cGameBoard, this->m_cMission))
						{
							// FirstMove = true
							if (!m_cPlayer->getFirstMove())
							{
								m_cPlayer->setFirstMove(true);
							}
						}
					}
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			Director::getInstance()->replaceScene(TransitionFade::create(1.f, StageModeMenu::createScene(), Color3B(0, 0, 0)));
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			if (m_bShowGameOver)
			{
				this->checkScore();
				this->menuCloseCallback(this);
			}
			else
			{
				this->menuCloseCallback(this);
			}
			break;
		}
	}
}

void TutorialModeGame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	m_bKeyPressed = false;
}

void TutorialModeGame::onMouseMove(cocos2d::Event * event)
{

}

void TutorialModeGame::onMouseUp(cocos2d::Event * event)
{

}

void TutorialModeGame::onMouseDown(cocos2d::Event * event)
{

}

void TutorialModeGame::onMouseScroll(cocos2d::Event * event)
{

}

std::string TutorialModeGame::getStepText(EASY_STEPS step)
{
	switch (step)
	{
	case STEP_1_0:
		return "Greetings Player! (Press Enter)";
		break;
	case STEP_1_1:
		return "Welcome to Square Line";
		break;
	case STEP_1_2:
		return "I'll teach you how to play the game!";
		break;
	case STEP_1_3:
		return "The Map made up of 12x12 Grids";
		break;
	case STEP_1_4:
		return "You will be spawned randomly on different maps";
		break;
	case STEP_1_5:
		return "You are currently spawned at one of the four starting points for this map";
		break;
	case STEP_1_6:
		return "The Red Square is your cursor and current location";
		break;
	case STEP_1_7:
		// Display Directional Keys at Player Speech
		return "First, move to the grid highlighted by the Yellow Square using the arrow keys";
		break;
	case STEP_1_8:
		return "Good Job! Now continue until you reach the blinking picture";
		break;
	case STEP_1_9:
		//x3 > x2
		return "Good Job! Now continue until you reach the blinking picture";
		break;
	case STEP_1_10:
		// x2 > x1
		return "Good Job! Now continue until you reach the blinking picture";
		break;
	case STEP_1_11:
		// x1 > x0
		return "Good Job! Now continue until you reach the blinking picture";
		break;
	case STEP_1_12:
		// y3 > y2
		return "Good Job! Now continue until you reach the blinking picture";
		break;
	case STEP_1_13:
		// y2 > y1
		return "Good! Now Hit Enter!";
		break;
	case STEP_1_14:
		return "MISSING STRING";
		break;
	case STEP_1_15:
		return "MISSING STRING";
		break;
	case STEP_1_16:
		return "MISISNG STRING";
		break;
	case STEP_1_17:
		// +1000
		return "This is how you earn score";
		break;
	case STEP_1_18:
		return "The selected grids will no longer be part of the board";
		break;
	case STEP_1_19:
		return "These boxes are now untargetable";
		break;
	case STEP_1_20:
		return "This is your new starting point";
		break;
	case STEP_1_21:
		return "You may move around in a 3x3 area shown by the Yellow Squares";
		break;
	case STEP_1_22:
		return "You'll be given a mission";
		break;
	case STEP_1_23:
		return "You'll have to form lines matching the mission";
		break;
	case STEP_1_24:
		return "So lets see you complete the mission";
		break;
	case STEP_1_25:
		return "Make 5 Lines made up of 6 Boxes";
		break;
	case STEP_1_26:
		return "Some NPCs will create obstacles in the map";
		break;
	case STEP_1_27:
		return "These obstacles are untargetable too";
		break;
	case STEP_1_28:
		return "Good Luck! (Press Enter again to continue) ";
		break;
	case STEP_2_0:
		return "I am the NPC for this stage!";
		break;
	case STEP_2_1:
		return "This is stage should look familiar to you...";
		break;
	case STEP_2_2:
		return "This stage is similar to Stage 1 except for a few differences";
		break;
	case STEP_2_3:
		return "This is your starting position";
		break;
	case STEP_2_4:
		return "There are a few obstacles already spawned in the map";
		break;
	case STEP_2_5:
		return "Try to complete the level!";
		break;
	case STEP_2_6:
		return "(Press Enter again to continue)";
		break;
	case STEP_3_0:
		return "(Press Enter again to continue)";
		break;
	}
	return "...";
}

void TutorialModeGame::setStage(EASY_STAGE i)
{
	this->m_eStage = i;
	// Steps
	switch (this->m_eStage)
	{
	case EASY_1:
		this->m_bInStep = true;
		this->m_eStep = STEP_1_0;
		break;
	case EASY_2:
		this->m_bInStep = true;
		this->m_eStep = STEP_2_0;
		break;
	case EASY_3:
		this->m_bInStep = true;
		this->m_eStep = STEP_3_0;
		break;
	}
}

TutorialModeGame::EASY_STAGE TutorialModeGame::getStage() 
{ 
	return this->m_eStage;
}

void TutorialModeGame::updateScore()
{
	auto tutorialUI = this->getChildByName("TutorialModeGame UI");
	if (tutorialUI)
	{
		auto scoreUI = tutorialUI->getChildByName("Score UI");
		if (scoreUI)
		{
			if (this->m_cNPC->getScoreLines())
			{
				if (m_cPlayer->getLinesMade() >= m_cNPC->getScoreList()[SCORE_REWARD_3])
				{
					m_bReward3 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 1"))->setTexture("UI/star.png");
				}
				if (m_cPlayer->getLinesMade() >= m_cNPC->getScoreList()[SCORE_REWARD_2])
				{
					m_bReward2 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 2"))->setTexture("UI/star.png");
				}
				if (m_cPlayer->getLinesMade() >= m_cNPC->getScoreList()[SCORE_REWARD_1])
				{
					m_bReward1 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 3"))->setTexture("UI/star.png");
				}
			}
			else
			{
				if (m_cPlayer->getScore() >= m_cNPC->getScoreList()[SCORE_REWARD_3])
				{
					m_bReward3 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 1"))->setTexture("UI/star.png");
				}
				if (m_cPlayer->getScore() >= m_cNPC->getScoreList()[SCORE_REWARD_2])
				{
					m_bReward2 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 2"))->setTexture("UI/star.png");
				}
				if (m_cPlayer->getScore() >= m_cNPC->getScoreList()[SCORE_REWARD_1])
				{
					m_bReward1 = true;
					dynamic_cast<Sprite*>(scoreUI->getChildByName("Blank Star 3"))->setTexture("UI/star.png");
				}
			}
		}
	}
}

void TutorialModeGame::checkScore()
{
	UserDefault * theUser = UserDefault::getInstance();

	switch (StageModeNPC::NPC(this->m_cNPC->getID()))
	{
	case StageModeNPC::NPC_BISON:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("avatarProgress1"))
			{
				theUser->setBoolForKey("avatarProgress1", true);
			}
		}
		if (m_bReward2)
		{

		}
		if (m_bReward1)
		{
			if (!theUser->getBoolForKey("EasyStage2Unlocked"))
			{
				theUser->setBoolForKey("EasyStage2Unlocked", true);
			}
		}
		break;
	case StageModeNPC::NPC_TOSCAYA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("EasyStage3Unlocked"))
			{
				theUser->setBoolForKey("EasyStage3Unlocked", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("avatarProgress2"))
			{
				theUser->setBoolForKey("avatarProgress2", true);
			}
		}
		if (m_bReward1)
		{
			if (!theUser->getBoolForKey("NormalStage1Unlocked"))
			{
				theUser->setBoolForKey("NormalStage1Unlocked", true);
			}
		}
		break;
	case StageModeNPC::NPC_CEILTA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("avatarProgress3"))
			{
				theUser->setBoolForKey("avatarProgress3", true);
			}
		}
		if (m_bReward2)
		{

		}
		if (m_bReward1)
		{

		}
		break;
	}
	theUser->flush();
}