#include "StageModeMenu.h"
#include "SimpleAudioEngine.h"

// Scenes
#include "StageModeGame.h"
#include "TutorialModeMenu.h"
#include "TutorialModeGame.h"

// Testing Scenes Transition
#include "TestScene.h"
#include "Start.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* StageModeMenu::createScene()
{	
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StageModeMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool StageModeMenu::init()
{
	// Init Layer
	if (!Layer::init())
	{
		return false;
	}
	this->setName("Stage Mode Menu"); // For Easier Referencing
	SimpleAudioEngine::getInstance()->stopAllEffects();
	this->m_eCurrentState = STATE_SELECT_LEVEL;
	this->m_eCurrentDifficulty = DIFF_NONE;
	this->m_iSelectedButton = 2;
	this->m_iSelectedLevel = 1;
	this->m_bAnimating = false;
	this->m_eAnimateDir = ANIMATE_NONE;
	this->m_bSpawned = false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->setBackground();

	// Create Mouse and Keyboard Listeners
	this->createListeners();

	// Create Title 
	auto title = Label::createWithTTF("Stage Mode", "fonts/arial.ttf", 64);
	title->setName("Stage Mode");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255), 3);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.9f)));
	this->addChild(title, 1);

	// Create the Menu (Level Select)
	this->createLevelMenu();

	//Enable Update Overrides
	this->scheduleUpdate();

	// BG Music
	SimpleAudioEngine::sharedEngine()->playEffect("FX/StageMode/main_theme.wav", false);

	return true;
}

void StageModeMenu::setBackground()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::string filePath;

	switch (m_eCurrentDifficulty)
	{
	case StageModeMenu::DIFF_EASY:
		if (FileUtils::getInstance()->isFileExist("Background/Background1.jpg"))
		{
			filePath = "Background/Background1.jpg";
		}
		break;
	case StageModeMenu::DIFF_NORMAL:
		if (FileUtils::getInstance()->isFileExist("Background/Background2.jpg"))
		{
			filePath = "Background/Background2.jpg";
		}
		// Set Background Color 
		break;
	case StageModeMenu::DIFF_HARD:
		if (FileUtils::getInstance()->isFileExist("Background/Background3.jpg"))
		{
			filePath = "Background/Background3.jpg";
		}
		break;
	default:
		if (FileUtils::getInstance()->isFileExist("Background/menu.png"))
		{
			filePath = "Background/menu.png";
		}
		break;
	}

	if (this->getChildByName("Background"))
	{
		this->removeChildByName("Background");
	}
	auto background = Sprite::create(filePath);
	background->setName("Background");
	background->setPosition(Vec2(visibleSize.width * MIDDLE_WIDTH_OFFSET, visibleSize.height * MIDDLE_HEIGHT_OFFSET));
	this->addChild(background, -100);
}

void StageModeMenu::update(float dt)
{
	this->handleMenuUpdates(dt);
}

void StageModeMenu::createLevelMenu()
{
	auto menu = this->getChildByName("Menu");

	if (!menu)
	{
		menu = Node::create();
		menu->setName("Menu");
	}

	auto title = Label::createWithTTF("Select Level", "fonts/arial.ttf", 32);
	title->setName("Select Level");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255), 2);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.6f)));
	menu->addChild(title, 1);

	// Create Buttons
	this->createLevelButtons(menu);

	// Set the Menu Node to be part of the scene
	this->addChild(menu);
}

void StageModeMenu::createLevelButtons(Node * menuNode)
{
	// Create the Cursor
	auto cursor = Sprite::create("ButtonSelected.png");
	cursor->setName("Cursor");
	cursor->setScale(GLOBAL_UI_SCALE);
	cursor->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.4f)));
	auto fadein = FadeIn::create(0.6f);
	auto fadeout = FadeOut::create(0.6f);
	auto animSeq = Sequence::create(fadeout, fadein, nullptr);
	cursor->runAction(RepeatForever::create(animSeq));
	this->addChild(cursor, 1);

	// Easy
	auto button = ui::Button::create("ButtonNormal.png", "ButtonPressed.png", "ButtonPressed.png");
	button->setName("Easy Button");
	button->setScale(GLOBAL_UI_SCALE);
	button->setTitleText("Easy");
	button->setTitleColor(Color3B(0, 0, 0));
	button->setTitleFontName("fonts/arial.ttf");
	button->setTitleFontSize(32.f);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.2f, (Director::getInstance()->getVisibleSize().height * 0.4f)));
	button->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->changeMenu(STATE_SELECT_STAGE, DIFF_EASY);
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});
	menuNode->addChild(button);

	// Normal
	button = ui::Button::create("ButtonNormal.png", "ButtonPressed.png", "ButtonPressed.png");
	button->setName("Normal Button");
	button->setScale(GLOBAL_UI_SCALE);
	button->setTitleText("Normal");
	button->setTitleColor(Color3B(0, 0, 0));
	button->setTitleFontName("fonts/arial.ttf");
	button->setTitleFontSize(32.f);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.4f)));
	button->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->changeMenu(STATE_SELECT_STAGE, DIFF_NORMAL);
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});
	menuNode->addChild(button);

	// Hard
	button = ui::Button::create("ButtonNormal.png", "ButtonPressed.png", "ButtonPressed.png");
	button->setName("Hard Button");
	button->setTitleText("Hard");
	button->setScale(GLOBAL_UI_SCALE);
	button->setTitleColor(Color3B(0, 0, 0));
	button->setTitleFontName("fonts/arial.ttf");
	button->setTitleFontSize(32.f);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.8f, (Director::getInstance()->getVisibleSize().height * 0.4f)));
	button->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->changeMenu(STATE_SELECT_STAGE, DIFF_HARD);
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});
	menuNode->addChild(button);
}

void StageModeMenu::createStageMenu()
{
	auto menu = this->getChildByName("Menu");

	if (!menu)
	{
		menu = Node::create();
		menu->setName("Menu");
	}

	auto title = Label::createWithTTF("Select Stage", "fonts/arial.ttf", 32);
	title->setName("Select Stage");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255), 2);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.7f)));
	menu->addChild(title, 1);

	// Create Buttons
	this->createStageButtons(menu);

	// Set the Menu Node to be part of the scene
	this->addChild(menu);
}

void StageModeMenu::createStageButtons(Node * menuNode)
{
	// Create the cursor
	auto cursor = Sprite::create("SelectCursor.png");
	cursor->setName("Cursor");
	cursor->setScale(0.375f * GLOBAL_UI_SCALE);
	if (m_eCurrentDifficulty == DIFF_EASY)
	{
		cursor->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.5f)));
	}
	else
	{
		cursor->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.6f)));
	}
	auto fadein = FadeIn::create(0.6f);
	auto fadeout = FadeOut::create(0.6f);
	auto animSeq = Sequence::create(fadeout, fadein, nullptr);
	cursor->runAction(RepeatForever::create(animSeq));
	this->addChild(cursor, 1);

	// Variables
	Sprite * mapSprite;
	float buttonOffset = 0.f;
	int counter = 1;

	// Based on current difficulty (level)
	switch (this->m_eCurrentDifficulty)
	{
	case DIFF_EASY: // 2 Stages + 1
		if (UserDefault::getInstance()->getBoolForKey("EasyStage3Unlocked"))
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				auto stageSprite = Sprite::create("easy.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.25f * (i + 1);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.4f)));
				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);
				menuNode->addChild(stageSprite, 1);
			}
		}
		else
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				auto stageSprite = Sprite::create("easy.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.125f + ((i + 1) * 0.25f);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.4f)));

				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);

				menuNode->addChild(stageSprite, 1);
			}
		}
		break;
	case DIFF_NORMAL: // 3 Stages + 1
		mapSprite = Sprite::create("Maps/normal/map1.png");
		mapSprite->setName("Map 1");
		mapSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.2f)));
		mapSprite->setScale(GLOBAL_UI_SCALE);
		menuNode->addChild(mapSprite, 1);

		if (UserDefault::getInstance()->getBoolForKey("NormalStage4Unlocked"))
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				auto stageSprite = Sprite::create("normal.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.2f * (i + 1);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));
				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);

				menuNode->addChild(stageSprite, 1);
			}
		}
		else
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				auto stageSprite = Sprite::create("normal.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.25f * (i + 1);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));
				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);

				menuNode->addChild(stageSprite, 1);
			}
		}
		break;
	case DIFF_HARD: // 2 Stages + 1
		mapSprite = Sprite::create("Maps/hard/map1.png");
		mapSprite->setName("Map 1");
		mapSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.2f)));
		mapSprite->setScale(GLOBAL_UI_SCALE);
		menuNode->addChild(mapSprite, 1);

		if (UserDefault::getInstance()->getBoolForKey("HardStage3Unlocked"))
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				auto stageSprite = Sprite::create("hard.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.25f * (i + 1);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));
				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);

				menuNode->addChild(stageSprite, 1);
			}
		}
		else
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				auto stageSprite = Sprite::create("hard.png");
				std::stringstream ss1;
				ss1 << i + 1;
				stageSprite->setName("Stage " + ss1.str());
				stageSprite->setScale(0.4f * GLOBAL_UI_SCALE);
				buttonOffset = 0.125f + ((i + 1) * 0.25f);
				stageSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * buttonOffset, (Director::getInstance()->getVisibleSize().height * 0.5f)));

				std::stringstream ss;
				ss << "Stage " << i + 1;
				auto title = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 48);
				title->setColor(Color3B(255, 255, 255));
				title->enableOutline(Color4B(0, 0, 0, 255), 5);
				title->setPosition(stageSprite->getContentSize().width * 0.5f, stageSprite->getContentSize().height * 0.5f);
				stageSprite->addChild(title, 1);

				menuNode->addChild(stageSprite, 1);
			}
		}
		break;
	}
}

void StageModeMenu::createMap(int i, bool right)
{
	std::stringstream dirCheck; // Check Directory

	switch (m_eCurrentDifficulty)
	{
	case DIFF_EASY:
		dirCheck << "Maps/easy";
		break;
	case DIFF_NORMAL:
		dirCheck << "Maps/normal";
		break;
	case DIFF_HARD:
		dirCheck << "Maps/hard";
		break;
	}

	if (FileUtils::getInstance()->isDirectoryExist(dirCheck.str()))
	{
		auto menu = this->getChildByName("Menu");
		if (menu)
		{
			Sprite * mapSprite;
			std::stringstream ss;
			ss << dirCheck.str() << "/map" << this->m_iSelectedLevel << ".png";
			if (FileUtils::getInstance()->isFileExist(ss.str()))
			{
				mapSprite = Sprite::create(ss.str()); // first 
				std::stringstream ss;
				ss << "Map " << this->m_iSelectedLevel;
				mapSprite->setName(ss.str());
				mapSprite->setScale(GLOBAL_UI_SCALE);
				if (right)
				{
					mapSprite->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, (Director::getInstance()->getVisibleSize().height * 0.2f)));
					auto moveTo = MoveTo::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, mapSprite->getPositionY()));
					auto animSeq2 = Sequence::create(moveTo, nullptr);
					mapSprite->runAction(animSeq2);
				}
				else
				{
					mapSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, (Director::getInstance()->getVisibleSize().height * 0.2f)));
					auto moveTo = MoveTo::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, mapSprite->getPositionY()));
					auto animSeq2 = Sequence::create(moveTo, nullptr);
					mapSprite->runAction(animSeq2);
				}
			}
			menu->addChild(mapSprite, 1);
		}
	}
	else
	{
		MessageBox(dirCheck.str().c_str(), "Directory not found");
	}
}

void StageModeMenu::changeMenu(MENU_STATUS currentState, DIFFICULTY diff)
{
	auto cursor = this->getChildByName("Cursor");
	if (cursor)
	{
		cursor->removeFromParentAndCleanup(true);
		cursor->release();
	}

	auto menu = this->getChildByName("Menu");
	this->m_eCurrentState = currentState;

	if (menu) // Checking for nullptr
	{
		switch (currentState)
		{
		case STATE_SELECT_LEVEL:
			//// Reset Difficulty
			//this->m_eCurrentDifficulty = DIFF_NONE;
			//// Remove Current Menu
			//menu->removeFromParentAndCleanup(true);
			//// Change Background
			//this->setBackground();
			//// Create Level Select Menu
			//this->createLevelMenu();
			Director::getInstance()->replaceScene(TransitionFade::create(1.f, Start::createScene(), Color3B(0, 0, 0)));
			break;
		case STATE_SELECT_STAGE:
			// Reset selected level
			this->m_iSelectedLevel = 1;
			// Set Difficulty
			this->m_eCurrentDifficulty = diff;
			// Remove Current Menu
			menu->removeFromParentAndCleanup(true);
			// Change Background
			this->setBackground();
			// Create New Menu
			this->createStageMenu();
			break;
		}
	}
}

void StageModeMenu::handleMenuUpdates(float dt)
{
	auto cursor = this->getChildByName("Cursor");
	std::stringstream ss;
	ss << m_iSelectedLevel;

	if (cursor)
	{
		// Animate Blinking
		auto menu = this->getChildByName("Menu");
		if (menu)
		{
			switch (m_eCurrentState)
			{
			case STATE_SELECT_LEVEL:
				switch (m_iSelectedButton)
				{
				case DIFF_EASY:
					cursor->setPosition(menu->getChildByName("Easy Button")->getPosition());
					break;
				case DIFF_NORMAL:
					cursor->setPosition(menu->getChildByName("Normal Button")->getPosition());
					break;
				case DIFF_HARD:
					cursor->setPosition(menu->getChildByName("Hard Button")->getPosition());
					break;
				default:
					break;
				}
				break;
			case STATE_SELECT_STAGE:
				auto cursorPos = menu->getChildByName("Stage " + ss.str())->getPosition();
				cursor->setPosition(Vec2(cursorPos.x, cursorPos.y - (menu->getChildByName("Stage " + ss.str())->getContentSize().height * 0.2f)));
				break;
			}
		}
	}

	if (m_bAnimating)
	{
		auto menu = this->getChildByName("Menu");
		if (menu)
		{
			std::stringstream ss;
			switch (m_eAnimateDir)
			{
			case ANIMATE_RIGHT:
				ss << "Map " << this->m_iSelectedLevel - 1; // To find previous avatar
				break;
			case ANIMATE_LEFT:
				ss << "Map " << this->m_iSelectedLevel + 1; // To find previous avatar
				break;
			}

			auto previousMap = menu->getChildByName(ss.str());

			if (previousMap)
			{
				MoveBy * move;
				Sequence * animSeq;

				auto removeRest = CallFunc::create([this, previousMap](){
					this->m_bAnimating = false;
					previousMap->removeFromParent();
					if (m_bSpawned) m_bSpawned = false;
				});

				switch (m_eAnimateDir)
				{
				case ANIMATE_LEFT:
					move = MoveBy::create(0.1f, Vec2(-Director::getInstance()->getVisibleSize().width * 0.5f, 0));
					if (!m_bSpawned)
					{
						m_bSpawned = true;
						// Create new map and spawn it to the right
						this->createMap(this->m_iSelectedLevel, false);
					}
					break;
				case ANIMATE_RIGHT:
					move = MoveBy::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, 0));
					if (!m_bSpawned)
					{
						m_bSpawned = true;
						// Create new map and spawn it to the left
						this->createMap(this->m_iSelectedLevel, true);
					}
					break;
				}
				animSeq = Sequence::create(move, removeRest, nullptr);
				previousMap->runAction(animSeq);
			}
		}
	}
}

void StageModeMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void StageModeMenu::createListeners()
{
	// Create Keyboard Listener
	auto kListener = EventListenerKeyboard::create();
	kListener->onKeyPressed = CC_CALLBACK_2(StageModeMenu::onKeyPressed, this);
	kListener->onKeyReleased = CC_CALLBACK_2(StageModeMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kListener, this);

	// Create Mouse Listener
	auto mListener = EventListenerMouse::create();
	mListener->onMouseMove = CC_CALLBACK_1(StageModeMenu::onMouseMove, this);
	mListener->onMouseUp = CC_CALLBACK_1(StageModeMenu::onMouseUp, this);
	mListener->onMouseDown = CC_CALLBACK_1(StageModeMenu::onMouseDown, this);
	mListener->onMouseScroll = CC_CALLBACK_1(StageModeMenu::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);
}

void StageModeMenu::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_LEVEL:
			if (this->m_iSelectedButton > 1)
			{
				this->m_iSelectedButton -= 1;
				SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
			}
			break;
		case STATE_SELECT_STAGE:
			if (!m_bAnimating)
			{
				if (m_iSelectedLevel > 1)
				{
					if (this->m_eCurrentDifficulty == DIFF_NORMAL || this->m_eCurrentDifficulty == DIFF_HARD)
					{
						this->m_bAnimating = true;
						this->m_eAnimateDir = ANIMATE_LEFT;
					}
					m_iSelectedLevel -= 1;
					SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
				}
			}
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_LEVEL:
			if (this->m_iSelectedButton < 3)
			{
				this->m_iSelectedButton += 1;
				SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
			}
			break;
		case STATE_SELECT_STAGE:
			if (!m_bAnimating)
			{
				switch (this->m_eCurrentDifficulty)
				{
				case DIFF_EASY:
					if (UserDefault::getInstance()->getBoolForKey("EasyStage3Unlocked"))
					{
						if (m_iSelectedLevel < 3)
						{
							m_iSelectedLevel += 1;
							SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
						}
					}
					else
					{
						if (m_iSelectedLevel < 2)
						{
							m_iSelectedLevel += 1;
							SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
						}
					}
					break;
				case DIFF_NORMAL:
					if (UserDefault::getInstance()->getBoolForKey("NormalStage4Unlocked"))
					{
						if (m_iSelectedLevel < 4)
						{
							this->m_bAnimating = true;
							this->m_eAnimateDir = ANIMATE_RIGHT;
							m_iSelectedLevel += 1;
							SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
						}
					}
					else
					if (m_iSelectedLevel < 3)
					{
						this->m_bAnimating = true;
						this->m_eAnimateDir = ANIMATE_RIGHT;
						m_iSelectedLevel += 1;
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
					}
					break;
				case DIFF_HARD:
					if (UserDefault::getInstance()->getBoolForKey("HardStage3Unlocked"))
					{
						if (m_iSelectedLevel < 3)
						{
							this->m_bAnimating = true;
							this->m_eAnimateDir = ANIMATE_RIGHT;
							m_iSelectedLevel += 1;
							SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
						}
					}
					else
					if (m_iSelectedLevel < 2)
					{
						this->m_bAnimating = true;
						this->m_eAnimateDir = ANIMATE_RIGHT;
						m_iSelectedLevel += 1;
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Beep_Ping.wav", false);
					}
					break;
				}
			}
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_LEVEL:
			this->changeMenu(STATE_SELECT_STAGE, (DIFFICULTY)m_iSelectedButton);
			m_iSelectedButton = 2;
			break;
		case STATE_SELECT_STAGE:
			if (m_eCurrentDifficulty == DIFF_EASY)
			{
				UserDefault * theUser = UserDefault::getInstance();
				Scene * gameScene;
				switch (this->m_iSelectedLevel)
				{
				case 1:
					// EASY STAGE 1 
					gameScene = TutorialModeGame::createScene(0, 1, GameBoard::BOARD_TYPE::TYPE_NORMAL, this->m_iSelectedLevel);
					Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
					SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					break;
				case 2:
					// EASY STAGE 2 
					if (!theUser->getBoolForKey("EasyStage2Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = TutorialModeGame::createScene(0, 2, GameBoard::BOARD_TYPE::TYPE_NORMAL, this->m_iSelectedLevel);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				case 3:
					// EASY STAGE 3
					if (!theUser->getBoolForKey("EasyStage3Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = TutorialModeGame::createScene(0, 3, GameBoard::BOARD_TYPE::TYPE_NORMAL, this->m_iSelectedLevel);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
						break;
					}
				default:
					auto gameScene = TutorialModeMenu::createScene(this->m_iSelectedLevel);
					Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
					break;
				}
			}
			else if (m_eCurrentDifficulty == DIFF_NORMAL)
			{
				UserDefault * theUser = UserDefault::getInstance();
				Scene * gameScene;
				switch (this->m_iSelectedLevel)
				{
				case 1:
					// NORMAL STAGE 1
					if (!theUser->getBoolForKey("NormalStage1Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				case 2:
					// NORMAL STAGE 2 
					if (!theUser->getBoolForKey("NormalStage2Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				case 3:
					// NORMAL STAGE 3
					if (!theUser->getBoolForKey("NormalStage3Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				case 4:
					// NORMAL STAGE 4
					if (!theUser->getBoolForKey("NormalStage4Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						auto gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				}
			}
			else if (m_eCurrentDifficulty == DIFF_HARD)
			{
				UserDefault * theUser = UserDefault::getInstance();
				Scene * gameScene;
				switch (this->m_iSelectedLevel)
				{
				case 1:
					// HARD STAGE 1
					gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
					Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
					SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					break;
				case 2:
					// HARD STAGE 2 
					if (!theUser->getBoolForKey("HardStage2Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				case 3:
					// HARD STAGE 3
					if (!theUser->getBoolForKey("HardStage3Unlocked"))
					{
						SimpleAudioEngine::sharedEngine()->playEffect("FX/Buzz.wav");
					}
					else
					{
						gameScene = StageModeGame::createScene(this->m_iSelectedLevel, (int)this->m_eCurrentDifficulty);
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
						SimpleAudioEngine::sharedEngine()->playEffect("FX/CrystalBell.wav");
					}
					break;
				}
			}
			else
			{
				MessageBox("How did you get here!? : No Difficulty Selected", "ERROR");
			}
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
		this->changeMenu(STATE_SELECT_LEVEL);
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		this->menuCloseCallback(this);
		break;
	}
}

void StageModeMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{

}

void StageModeMenu::onMouseMove(cocos2d::Event * event)
{

}

void StageModeMenu::onMouseUp(cocos2d::Event * event)
{

}

void StageModeMenu::onMouseDown(cocos2d::Event * event)
{

}

void StageModeMenu::onMouseScroll(cocos2d::Event * event)
{

}