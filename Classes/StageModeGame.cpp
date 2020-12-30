#include "StageModeGame.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#include "StageModeMenu.h"
#include "AIComponent.h"

Scene* StageModeGame::createScene(int level, int diff)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StageModeGame::create();

	// Create Game Board and Relevant Info
	layer->createBoard(level, diff);
	layer->spawnPlayer(level, diff);
	layer->spawnEnemy(level, diff);
	layer->createUI();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StageModeGame::init()
{
	// Init Layer
	if (!Layer::init())
	{
		return false;
	}
	this->setName("Stage Mode Game"); // For Easier Referencing
	SimpleAudioEngine::getInstance()->stopAllEffects();
	// Gameplay Variables Init
	m_v2PlayerPositionIndex.setZero();
	m_ePlayerPP = GameBoard::PP_NONE;
	m_v2EnemyPositionIndex.setZero();
	m_eEnemyPP = GameBoard::PP_NONE;
	m_bKeyPressed = false;
	m_bCursorToggled = false;
	m_bShowGameOver = false;
	m_bReward1 = false;
	m_bReward2 = false;
	m_bReward3 = false;

	// Mission
	m_cMission = new Mission();

	// Timer
	this->m_cTimer = new StageModeTimer(60.f); // 1 Minute
	this->m_cTimer->start();

	// Create Mouse and Keyboard Listeners
	this->createListeners();

	//Enable Update Overrides
	this->scheduleUpdate();

	return true;
}

void StageModeGame::update(float dt)
{
	auto stageUI = this->getChildByName("StageModeGame UI");
	if (stageUI)
	{
		auto npcText = dynamic_cast<Label*>(stageUI->getChildByName("NPC Speech Bubble")->getChildByName("NPC Speech Text"));
		//auto playerText = dynamic_cast<Label*>(stageUI->getChildByName("Player Speech Bubble")->getChildByName("Player Speech Text"));
		if (npcText)
		{
			npcText->setString(this->m_cNPC->getText());
		}
		auto mission = dynamic_cast<Label*>(stageUI->getChildByName("Mission Name"));
		if (mission)
		{
			std::stringstream ss1;
			switch (this->m_cMission->getMissionType())
			{
			case Mission::MISSION_LINEANDBOXES:
				if (this->m_cMission->getMinimumRequired())
				{
					ss1 << "Mission : Create " << m_cMission->getLines() << " Lines using " << m_cMission->getMinimumBoxes() << " - "<< m_cMission->getBoxes() << " Boxes";
					mission->setString(ss1.str());
				}
				else
				{
					ss1 << "Mission : Create " << m_cMission->getLines() << " Lines using " << m_cMission->getBoxes() << " Boxes";
					mission->setString(ss1.str());
				}
				break;
			case Mission::MISSION_CUSTOM:
				if (this->m_cNPC->getAIComponent()->getFindIcon())
				{
					// Only Update Text if IconID changes
					if (m_cNPC->getAIComponent()->getIconToFind() != this->m_cMission->getIconID())
					{
						this->m_cMission->setIconID(m_cNPC->getAIComponent()->getIconToFind());
						mission->setString(m_cMission->getMissionText());
						mission->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * MIDDLE_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * MISSION_HEIGHT_TEXT_OFFSET)));
						std::stringstream filePath;
						filePath << "Icons/icon" << m_cNPC->getAIComponent()->getIconToFind() << ".png";
						if (mission->getChildByName("Mission Sprite"))
						{
							mission->removeChildByName("Mission Sprite");
						}
						auto missionSprite = Sprite::create(filePath.str());
						missionSprite->setName("Mission Sprite");
						missionSprite->setScale(0.7f);
						missionSprite->setPosition(Vec2(mission->getContentSize().width * MIDDLE_WIDTH_OFFSET, mission->getContentSize().height - missionSprite->getContentSize().height));
						mission->addChild(missionSprite, 1);
					}
				}
				else
				{
					mission->setString(m_cMission->getMissionText());
				}
				break;
			}
		}

		this->m_cTimer->update(dt, m_cNPC->getAIComponent());

		if (m_cPlayer)
		{
			if (m_cPlayer->getStunDuration() > 0)
			{
				m_cPlayer->setStunDuration(m_cPlayer->getStunDuration() - dt);
				if (m_cPlayer->getStunDuration() <= 0)
				{
					auto playerAvatar = this->getChildByName("Player Node")->getChildByName("Default Avatar");
					if (playerAvatar)
					{
						playerAvatar->setColor(Color3B::WHITE);
					}
				}
			}
		}

		if (m_cNPC)
		{
			if (m_cTimer->getStart())
			{
				m_cNPC->runPattern(this->m_cGameBoard, this->m_cPlayer, dt);
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

void StageModeGame::createBoard(int level, int diff)
{
	this->setBackground(StageModeMenu::DIFFICULTY(diff),level);
	switch (StageModeMenu::DIFFICULTY(diff))
	{
	case StageModeMenu::DIFF_EASY:
		this->m_cGameBoard = new GameBoard();
		break;
	case StageModeMenu::DIFF_NORMAL:
		switch (level)
		{
		case 1:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/normal_theme.wav");
			this->m_cGameBoard = new GameBoard(GameBoard::TYPE_CIRCLE);
			this->m_cMission->setLines(999);
			this->m_cMission->setBoxes(999);
			this->m_cMission->setFindIcon(true);
			this->m_cMission->setMissionType(Mission::MISSION_CUSTOM);
			this->m_cMission->setMissionText("Can you find this picture?");
			break;
		case 2:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/normal_theme.wav");
			this->m_cGameBoard = new GameBoard(GameBoard::TYPE_ACORN);
			this->m_cMission->setBoxes(6);
			this->m_cMission->setLines(999);
			this->m_cMission->setMissionType(Mission::MISSION_CUSTOM);
			this->m_cMission->setMissionText("Create as many pairs as possible");
			break;
		case 3:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/normal_theme.wav");
			this->m_cGameBoard = new GameBoard(GameBoard::TYPE_HEART);
			this->m_cMission->setBoxes(6);
			this->m_cMission->setLines(5);
			this->m_cMission->setMissionType(Mission::MISSION_LINEANDBOXES);
			break;
		case 4:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/hidden_theme.wav");
			this->m_cGameBoard = new GameBoard(GameBoard::TYPE_NORMAL);
			this->m_cMission->setBoxes(6);
			this->m_cMission->setLines(999);
			this->m_cMission->setMissionType(Mission::MISSION_CUSTOM);
			this->m_cMission->setMissionText("Create as many pairs as possible");
		default:
			this->m_cGameBoard = new GameBoard();
			break;
		}
		break;
	case StageModeMenu::DIFF_HARD:
		switch (level)
		{
		case 1:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/hard_theme_1.wav");
			this->m_cGameBoard = new GameBoard();
			this->m_cMission->setLines(6);
			this->m_cMission->setBoxes(6);
			this->m_cMission->setMinimumBoxes(3);
			this->m_cMission->setMinimumRequired(true);
			this->m_cMission->setMissionType(Mission::MISSION_LINEANDBOXES);
			break;
		case 2:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/hard_theme_2.wav");
			this->m_cGameBoard = new GameBoard();
			this->m_cMission->setLines(999);
			this->m_cMission->setBoxes(999);
			this->m_cMission->setMinimumBoxes(6);
			this->m_cMission->setMinimumRequired(true);
			this->m_cMission->setMissionType(Mission::MISSION_CUSTOM);
			this->m_cMission->setMissionText("Make as many lines as possible using more than 5 boxes");
			break;
		case 3:
			SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/hard_theme_3.wav");
			this->m_cGameBoard = new GameBoard(GameBoard::TYPE_CLOVER);
			this->m_cMission->setLines(999);
			this->m_cMission->setBoxes(999);
			this->m_cMission->setMinimumBoxes(4);
			this->m_cMission->setMinimumRequired(true);
			this->m_cMission->setFindIcon(true);
			this->m_cMission->setMissionType(Mission::MISSION_CUSTOM);
			this->m_cMission->setInLine(true);
			this->m_cMission->setMissionText("Make as many lines as possible. Include this picture, using > 3 boxes");
			break;
		}
		break;
	}
	this->m_cGameBoard->setName("GameBoard");
	this->m_cGameBoard->setPositionY(-Director::getInstance()->getVisibleSize().height * GAMEBOARD_HEIGHT_OFFSET);
	this->addChild(m_cGameBoard, -1);
}

void StageModeGame::setBackground(int diff, int level)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::string filePath;

	switch (StageModeMenu::DIFFICULTY(diff))
	{
	case StageModeMenu::DIFF_EASY: // SHOULD NOT BE REACHED
		if (FileUtils::getInstance()->isFileExist("Background/Background1.jpg"))
		{
			filePath = "Background/Background1.jpg";
		}
		break;
	case StageModeMenu::DIFF_NORMAL:
		switch (level)
		{
		case 1:
			if (FileUtils::getInstance()->isFileExist("Background/normal_1.png"))
			{
				filePath = "Background/normal_1.png";
			}
			break;
		case 2:
			if (FileUtils::getInstance()->isFileExist("Background/normal_2.png"))
			{
				filePath = "Background/normal_2.png";
			}
			break;
		case 3:
			if (FileUtils::getInstance()->isFileExist("Background/normal_3.png"))
			{
				filePath = "Background/normal_3.png";
			}
			break;
		case 4:
			if (FileUtils::getInstance()->isFileExist("Background/Background3.jpg"))
			{
				filePath = "Background/Background3.jpg";
			}
			break;
		}
		break;
	case StageModeMenu::DIFF_HARD:
		switch (level)
		{
		case 1:
			if (FileUtils::getInstance()->isFileExist("Background/hard.png"))
			{
				filePath = "Background/hard.png";
			}
			break;
		case 2:
			if (FileUtils::getInstance()->isFileExist("Background/hard.png"))
			{
				filePath = "Background/hard.png";
			}
			break;
		case 3:
			if (FileUtils::getInstance()->isFileExist("Background/hard_hidden.png"))
			{
				filePath = "Background/hard_hidden.png";
			}
			break;
		}
		break;
	default:
		MessageBox("Invalid Difficulty Selected", "ERROR");
		break;
	}

	auto background = Sprite::create(filePath);
	background->setPosition(Vec2(visibleSize.width * MIDDLE_WIDTH_OFFSET, visibleSize.height * MIDDLE_HEIGHT_OFFSET));
	this->addChild(background, -100);
}

void StageModeGame::createUI()
{
	auto uiNode = Node::create();
	uiNode->setName("StageModeGame UI");

	/******* Game Information*******/
	// Timer
	uiNode->addChild(m_cTimer, 1);

	// Mission
	auto mission = Label::createWithTTF("", "fonts/arial.ttf", 24);
	mission->setName("Mission Name");
	mission->setColor(Color3B(255, 255, 255));
	mission->enableOutline(Color4B(0, 0, 0, 255), 2);
	mission->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * MIDDLE_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * MISSION_HEIGHT_TEXT_OFFSET2)));
	uiNode->addChild(mission, 1);

	/********* Player UI *********/
	// Avatar
	auto avatarNode1 = Node::create();
	avatarNode1->setName("Player Node");
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
	/*std::stringstream ss3;
	ss3 << "Lines Made : " << m_cPlayer->getLinesMade();
	auto line = Label::createWithTTF(ss3.str(), "fonts/arial.ttf", 24);
	line->setName("Player Lines");
	line->setColor(Color3B(255, 255, 255));
	line->enableOutline(Color4B(0, 0, 0, 255), 2);
	line->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * PLAYER_WIDTH_OFFSET, (Director::getInstance()->getVisibleSize().height * PLAYER_LINES_HEIGHT_OFFSET)));
	uiNode->addChild(line, 1);*/

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
	default:
		s = this->m_cNPC->getText();
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

void StageModeGame::createGameOverMenu(std::string s)
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

Sprite * StageModeGame::createAvatar(int avatarID)
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

void StageModeGame::spawnPlayer(int level, int diff)
{
	// Create Player
	this->m_cPlayer = new StageModePlayer("Josh");

	if (m_cGameBoard)
	{
		int randBoardPiece;
		int rand2;

		switch ((StageModeMenu::DIFFICULTY)diff)
		{
		case StageModeMenu::DIFF_EASY:
			MessageBox("How did you get here? Easy Difficulty Selected in Normal/Hard Mode", "ERROR");
			break;
		case StageModeMenu::DIFF_NORMAL:
			switch (STAGE(level))
			{
			case STAGE_NORMAL_1:
				// Select one of the 4 corners
				randBoardPiece = cocos2d::random(0, 3);
				switch (randBoardPiece)
				{
				case 0:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(2, 1)->getPosition());
					m_cPlayer->setIndex(Vec2(2, 1));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(2, 1));
					m_cPlayer->setOldIndex(Vec2(2, 1));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->addChild(m_cPlayer, 1);
					break;
				case 1:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(2, 2)->getPosition());
					m_cPlayer->setIndex(Vec2(2, 2));
					m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(2, 2));
					m_cPlayer->setOldIndex(Vec2(2, 2));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->addChild(m_cPlayer, 1);
					break;
				case 2:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 2)->getPosition());
					m_cPlayer->setIndex(Vec2(1, 2));
					m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(1, 2));
					m_cPlayer->setOldIndex(Vec2(1, 2));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
					break;
				case 3:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(1, 1)->getPosition());
					m_cPlayer->setIndex(Vec2(1, 1));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(1, 1));
					m_cPlayer->setOldIndex(Vec2(1, 1));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
					break;
				}
				break;
			case STAGE_NORMAL_2:
				// Select one of the 2 corners
				randBoardPiece = cocos2d::random(0, 1);
				switch (randBoardPiece)
				{
				case 0:
					rand2 = cocos2d::random(0, 1);
					switch (rand2)
					{
					case 0:
						m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(2, 0)->getPosition());
						m_cPlayer->setIndex(Vec2(2, 0));
						m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
						m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(2, 0));
						m_cPlayer->setOldIndex(Vec2(2, 0));
						this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
						break;
					case 1:
						m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(3, 1)->getPosition());
						m_cPlayer->setIndex(Vec2(3, 1));
						m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
						m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(3, 1));
						m_cPlayer->setOldIndex(Vec2(3, 1));
						this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
						break;
					}
					break;
				case 1:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(0, 3)->getPosition());
					m_cPlayer->setIndex(Vec2(0, 3));
					m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(0, 3));
					m_cPlayer->setOldIndex(Vec2(0, 3));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->addChild(m_cPlayer, 1);
					break;
				}
				break;
			case STAGE_NORMAL_3:
				// Select one of the 3 corners
				randBoardPiece = cocos2d::random(0, 2);
				switch (randBoardPiece)
				{
				case 0:
					rand2 = cocos2d::random(0, 1);
					switch (rand2)
					{
					case 0:
						m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(1, 3)->getPosition());
						m_cPlayer->setIndex(Vec2(1, 3));
						m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOM);
						m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(1, 3));
						m_cPlayer->setOldIndex(Vec2(1, 3));
						this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->addChild(m_cPlayer, 1);
						break;
					case 1:
						m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(2, 3)->getPosition());
						m_cPlayer->setIndex(Vec2(2, 3));
						m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOM);
						m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->getGrid(2, 3));
						m_cPlayer->setOldIndex(Vec2(2, 3));
						this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOM)->addChild(m_cPlayer, 1);
						break;
					}
					break;
				case 1:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(2, 0)->getPosition());
					m_cPlayer->setIndex(Vec2(2, 0));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(2, 0));
					m_cPlayer->setOldIndex(Vec2(2, 0));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->addChild(m_cPlayer, 1);
					break;
				case 2:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(0, 0)->getPosition());
					m_cPlayer->setIndex(Vec2(0, 0));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(0, 0));
					m_cPlayer->setOldIndex(Vec2(0, 0));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
					break;
				}
				break;
			case STAGE_NORMAL_4:
				randBoardPiece = cocos2d::random(0, 3);
				switch (randBoardPiece)
				{
				case 0:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(3, 0)->getPosition());
					m_cPlayer->setIndex(Vec2(3, 0));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(3, 0));
					m_cPlayer->setOldIndex(Vec2(3, 0));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
					break;
				case 1:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(0, 0)->getPosition());
					m_cPlayer->setIndex(Vec2(0, 0));
					m_cPlayer->setPiecePosition(GameBoard::PP_TOPLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(0, 0));
					m_cPlayer->setOldIndex(Vec2(0, 0));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->addChild(m_cPlayer, 1);
					break;
				case 2:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 3)->getPosition());
					m_cPlayer->setIndex(Vec2(3, 3));
					m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 3));
					m_cPlayer->setOldIndex(Vec2(3, 3));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
					break;
				case 3:
					m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(0, 3)->getPosition());
					m_cPlayer->setIndex(Vec2(0, 3));
					m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMLEFT);
					m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(0, 3));
					m_cPlayer->setOldIndex(Vec2(0, 3));
					this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->addChild(m_cPlayer, 1);
					break;
				}
				break;
			}
			break;
		case StageModeMenu::DIFF_HARD:
			randBoardPiece = cocos2d::random(0, 3);
			switch (randBoardPiece)
			{
			case 0:
				m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(0, 1)->getPosition());
				m_cPlayer->setIndex(Vec2(0, 1));
				m_cPlayer->setPiecePosition(GameBoard::PP_TOPLEFT);
				m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->getGrid(0, 1));
				m_cPlayer->setOldIndex(Vec2(0, 1));
				this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPLEFT)->addChild(m_cPlayer, 1);
				break;
			case 1:
				m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(1, 3)->getPosition());
				m_cPlayer->setIndex(Vec2(1, 3));
				m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMLEFT);
				m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->getGrid(1, 3));
				m_cPlayer->setOldIndex(Vec2(1, 3));
				this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMLEFT)->addChild(m_cPlayer, 1);
				break;
			case 2:
				m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 2)->getPosition());
				m_cPlayer->setIndex(Vec2(3, 2));
				m_cPlayer->setPiecePosition(GameBoard::PP_BOTTOMRIGHT);
				m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->getGrid(3, 2));
				m_cPlayer->setOldIndex(Vec2(3, 2));
				this->m_cGameBoard->getBoardPiece(GameBoard::PP_BOTTOMRIGHT)->addChild(m_cPlayer, 1);
				break;
			case 3:
				m_cPlayer->setPosition(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(2, 0)->getPosition());
				m_cPlayer->setIndex(Vec2(2, 0));
				m_cPlayer->setPiecePosition(GameBoard::PP_TOPRIGHT);
				m_cPlayer->setOldGrid(this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->getGrid(2, 0));
				m_cPlayer->setOldIndex(Vec2(2, 0));
				this->m_cGameBoard->getBoardPiece(GameBoard::PP_TOPRIGHT)->addChild(m_cPlayer, 1);
				break;
			}
			break;
		}
	}
	else
	{
		MessageBox("Could not spawn player : GameBoard not created", "ERROR");
	}
}

void StageModeGame::spawnEnemy(int level, int diff)
{
	switch ((StageModeMenu::DIFFICULTY)diff)
	{
	case StageModeMenu::DIFF_EASY:
		MessageBox("How did you get here? Easy Difficulty Selected in Normal/Hard Mode", "ERROR");
		break;
	case StageModeMenu::DIFF_NORMAL:
		switch (level)
		{
		case 1:
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_HAJIME);
			break;
		case 2:
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_POKA);
			break;
		case 3: 
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_TSUBASA);
			break;
		case 4: // Hidden Stage
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_CHINGCHING);
			break;
		}
		break;
	case StageModeMenu::DIFF_HARD:
		switch (level)
		{
		case 1:
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_ASELLIA);
			break;
		case 2:
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_YORSHTA);
			break;
		case 3: // Hidden Stage
			this->m_cNPC = new StageModeNPC(StageModeNPC::NPC_ABYSSASELLIA);
			break;
		}
		break;
	}
	this->addChild(this->m_cNPC);
}

void StageModeGame::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void StageModeGame::createListeners()
{
	// Create Keyboard Listener
	auto kListener = EventListenerKeyboard::create();
	kListener->onKeyPressed = CC_CALLBACK_2(StageModeGame::onKeyPressed, this);
	kListener->onKeyReleased = CC_CALLBACK_2(StageModeGame::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kListener, this);

	// Create Mouse Listener
	auto mListener = EventListenerMouse::create();
	mListener->onMouseMove = CC_CALLBACK_1(StageModeGame::onMouseMove, this);
	mListener->onMouseUp = CC_CALLBACK_1(StageModeGame::onMouseUp, this);
	mListener->onMouseDown = CC_CALLBACK_1(StageModeGame::onMouseDown, this);
	mListener->onMouseScroll = CC_CALLBACK_1(StageModeGame::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);
}

void StageModeGame::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
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
					if (m_cPlayer->getStunDuration() <= 0)
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
					if (m_cPlayer->getStunDuration() <= 0)
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
					if (m_cPlayer->getStunDuration() <= 0)
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
					if (m_cPlayer->getStunDuration() <= 0)
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

void StageModeGame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	m_bKeyPressed = false;
}

void StageModeGame::onMouseMove(cocos2d::Event * event)
{

}

void StageModeGame::onMouseUp(cocos2d::Event * event)
{

}

void StageModeGame::onMouseDown(cocos2d::Event * event)
{

}

void StageModeGame::onMouseScroll(cocos2d::Event * event)
{

}

void StageModeGame::setStage(int level, int diff)
{
	switch ((StageModeMenu::DIFFICULTY)diff)
	{
	case StageModeMenu::DIFF_EASY:
		MessageBox("How did you get here? Easy Difficulty Selected in Normal/Hard Mode", "ERROR");
		break;
	case StageModeMenu::DIFF_NORMAL:
		switch (level)
		{
		case 1:
			this->m_eStage = STAGE_NORMAL_1;
			break;
		case 2:
			this->m_eStage = STAGE_NORMAL_2;
			break;
		case 3:
			this->m_eStage = STAGE_NORMAL_3;
			break;
		case 4: // Hidden Stage
			this->m_eStage = STAGE_NORMAL_4;
			break;
		}
		break;
	case StageModeMenu::DIFF_HARD:
		switch (level)
		{
		case 1:
			this->m_eStage = STAGE_HARD_1;
			break;
		case 2:
			this->m_eStage = STAGE_HARD_2;
			break;
		case 3: // Hidden Stage
			this->m_eStage = STAGE_HARD_3;
			break;
		}
		break;
	}
}

StageModeGame::STAGE StageModeGame::getStage()
{
	return this->m_eStage;
}

std::string StageModeGame::getStepText(STAGE step)
{
	switch (step)
	{
	default:
		break;
	}
	return "...";
}

void StageModeGame::updateScore()
{
	auto stageUI = this->getChildByName("StageModeGame UI");
	if (stageUI)
	{
		auto scoreUI = stageUI->getChildByName("Score UI");
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

void StageModeGame::checkScore()
{
	UserDefault * theUser = UserDefault::getInstance();

	switch (StageModeNPC::NPC(this->m_cNPC->getID()))
	{
	case StageModeNPC::NPC_HAJIME:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("mapProgress1"))
			{
				theUser->setBoolForKey("mapProgress1", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("avatarProgress4"))
			{
				theUser->setBoolForKey("avatarProgress4", true);
			}
		}
		if (m_bReward1)
		{
			if (!theUser->getBoolForKey("NormalStage2Unlocked"))
			{
				theUser->setBoolForKey("NormalStage2Unlocked", true);
			}
		}
		break;
	case StageModeNPC::NPC_POKA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("mapProgress2"))
			{
				theUser->setBoolForKey("mapProgress2", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("avatarProgress5"))
			{
				theUser->setBoolForKey("avatarProgress5", true);
			}
		}
		if (m_bReward1)
		{
			if (!theUser->getBoolForKey("NormalStage3Unlocked"))
			{
				theUser->setBoolForKey("NormalStage3Unlocked", true);
			}
		}
		break;
	case StageModeNPC::NPC_TSUBASA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("NormalStage4Unlocked"))
			{
				theUser->setBoolForKey("NormalStage4Unlocked", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("avatarProgress6"))
			{
				theUser->setBoolForKey("avatarProgress6", true);
			}
		}
		if (m_bReward1)
		{
			if (!theUser->getBoolForKey("mapProgress3"))
			{
				theUser->setBoolForKey("mapProgress3", true);
			}
		}
		break;
	case StageModeNPC::NPC_CHINGCHING:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("avatarProgress7"))
			{
				theUser->setBoolForKey("avatarProgress7", true);
			}
		}
		if (m_bReward2)
		{

		}
		if (m_bReward1)
		{

		}
		break;
	case StageModeNPC::NPC_ASELLIA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("avatarProgress8"))
			{
				theUser->setBoolForKey("avatarProgress8", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("HardStage2Unlocked"))
			{
				theUser->setBoolForKey("HardStage2Unlocked", true);
			}
		}
		if (m_bReward1)
		{

		}
		break;
	case StageModeNPC::NPC_YORSHTA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("HardStage3Unlocked"))
			{
				theUser->setBoolForKey("HardStage3Unlocked", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("avatarProgress9"))
			{
				theUser->setBoolForKey("avatarProgress9", true);
			}
		}
		if (m_bReward1)
		{

		}
		break;
	case StageModeNPC::NPC_ABYSSASELLIA:
		if (m_bReward3)
		{
			if (!theUser->getBoolForKey("avatarProgress10"))
			{
				theUser->setBoolForKey("avatarProgress10", true);
			}
		}
		if (m_bReward2)
		{
			if (!theUser->getBoolForKey("mapProgress4"))
			{
				theUser->setBoolForKey("mapProgress4", true);
			}
		}
		if (m_bReward1)
		{

		}
		break;
	}
	theUser->flush();
}