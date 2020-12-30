#include "TutorialModeMenu.h"
#include "SimpleAudioEngine.h"

// Scenes
#include "TutorialModeGame.h"

// Testing Scenes Transition
#include "TestScene.h"
#include "StageModeMenu.h"

USING_NS_CC;

Scene* TutorialModeMenu::createScene(int level)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TutorialModeMenu::create();
	layer->setLevel(level);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool TutorialModeMenu::init()
{
	// Init Layer
	if (!Layer::init())
	{
		return false;
	}
	this->setName("Tutorial Mode Menu"); // For Easier Referencing
	this->m_eCurrentState = STATE_SELECT_AVATAR1;
	this->m_iP1SelectedAvatar = 1;
	this->m_iP2SelectedAvatar = 1;
	this->m_iPreviousAvatar = 1;
	this->m_iSelectedMap = 1;
	this->m_bAnimating = false;
	this->m_eAnimateDir = ANIMATE_NONE;
	this->m_iTotalAvatars = 8; // How many avatars unlocked, 5 for now.
	this->m_bSpawned = false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Set Background Color 
	auto background = cocos2d::LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background,-1);

	// Create Mouse and Keyboard Listeners
	this->createListeners();

	// Create Title 
	auto title = Label::createWithTTF("Tutorial (Classic Mode)", "fonts/arial.ttf", 64);
	title->setName("Tutorial");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255), 3);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.9f)));
	this->addChild(title, 1);

	// Create the Menu (Avatar Select)
	this->createAvatarMenu();

	//Enable Update Overrides
	this->scheduleUpdate();

	return true;
}

void TutorialModeMenu::update(float dt)
{
	this->handleMenuUpdates(dt);
}

void TutorialModeMenu::createAvatarMenu()
{
	auto menu = this->getChildByName("Menu");

	if (!menu)
	{
		menu = Node::create();
		menu->setName("Menu");
	}

	auto title = Label::createWithTTF("Select Avatar", "fonts/arial.ttf", 32);
	title->setName("Select Avatar");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255),2);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.6f)));
	menu->addChild(title, 1);

	auto stage = Label::createWithTTF("Avatar : ", "fonts/arial.ttf", 32);
	stage->setName("Avatar No.");
	stage->setColor(Color3B(255, 255, 255));
	stage->enableOutline(Color4B(0, 0, 0, 255), 2);
	stage->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.2f)));
	menu->addChild(stage, 1);

	auto player = Label::createWithTTF("Player ", "fonts/arial.ttf", 32);
	player->setName("Player");
	player->setColor(Color3B(255, 255, 255));
	player->enableOutline(Color4B(0, 0, 0, 255), 2);
	player->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.69f)));
	menu->addChild(player, 1);

	// Create Buttons
	this->createAvatarButtons();

	// Set the Menu Node to be part of the scene
	this->addChild(menu);
}

void TutorialModeMenu::createAvatarButtons()
{
	auto avatar = Sprite::create("Avatars/avatar1/frame_0.png"); // first 
	avatar->setName("Avatar 1");
	avatar->setScale(GLOBAL_UI_SCALE);
	avatar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.4f)));

	Vector<SpriteFrame*> animFrames;
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_0.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_1.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_2.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_3.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_4.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_5.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_6.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_7.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_8.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_9.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_10.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_11.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_12.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));
	animFrames.pushBack(SpriteFrame::create("Avatars/avatar1/frame_13.png", Rect(0, 0, avatar->getContentSize().width, avatar->getContentSize().width)));

	// create the animation out of the frames
	Animation* animation = Animation::createWithSpriteFrames(animFrames,0.1f);
	Animate* animate = Animate::create(animation);
	avatar->runAction(RepeatForever::create(animate));

	this->addChild(avatar, 1);
}

void TutorialModeMenu::createMapMenu()
{
	auto menu = this->getChildByName("Menu");

	if (!menu)
	{
		menu = Node::create();
		menu->setName("Menu");
	}

	auto title = Label::createWithTTF("Select Map", "fonts/arial.ttf", 32);
	title->setName("Select Map");
	title->setColor(Color3B(255, 255, 255));
	title->enableOutline(Color4B(0, 0, 0, 255), 2);
	title->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.6f)));
	menu->addChild(title, 1);

	//auto stage = Label::createWithTTF("Map : ", "fonts/arial.ttf", 32);
	//stage->setName("Map No.");
	//stage->setColor(Color3B(255, 255, 255));
	//stage->enableOutline(Color4B(0, 0, 0, 255), 2);
	//stage->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.2f)));
	//menu->addChild(stage, 1);

	// Create Buttons
	this->createMapButtons();

	// Set the Menu Node to be part of the scene
	this->addChild(menu);
}

void TutorialModeMenu::createMapButtons()
{
	/* SHOULD SHOW ALL UNLOCKED MAPS */
	auto map = Sprite::create("Maps/easy/map1.png");  
	map->setName("Map 1");
	map->setScale(GLOBAL_UI_SCALE);
	map->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, (Director::getInstance()->getVisibleSize().height * 0.4f)));
	this->addChild(map, 1);
}

void TutorialModeMenu::changeMenu(MENU_STATUS currentState)
{
	auto menu = this->getChildByName("Menu");
	this->m_eCurrentState = currentState;

	if (menu) // Checking for nullptr
	{
		switch (currentState)
		{
		case STATE_SELECT_MAP:
			// Delete Existing Avatar Sprite
			std::stringstream ss;
			ss << "Avatar " << m_iP2SelectedAvatar;
			auto cleanAvatar = this->getChildByName(ss.str());
			if (cleanAvatar)
			{
				cleanAvatar->removeFromParent();
			}
			// Remove Current Menu
			menu->removeFromParentAndCleanup(true);
			// Create Level Select Menu
			this->createMapMenu();
			break;
		}
	}
}

void TutorialModeMenu::handleMenuUpdates(float dt)
{
	if (m_bAnimating)
	{
		switch (m_eCurrentState)
		{
		case STATE_SELECT_AVATAR1:
			break;
		case STATE_SELECT_AVATAR2:
			break;
		case STATE_SELECT_MAP:
			break;
		}
		int previousAvatar = 0;
		std::stringstream ss;
		ss << "Avatar " << this->m_iPreviousAvatar; // To find previous avatar
		auto oldAvatar = this->getChildByName(ss.str());
		if (oldAvatar)
		{
			MoveBy * move;
			Sequence * animSeq;

			auto removeRest = CallFunc::create([this, oldAvatar](){
				this->m_bAnimating = false;
				oldAvatar->removeFromParent();
				if (m_bSpawned) m_bSpawned = false;
			});

			switch (m_eAnimateDir)
			{
			case ANIMATE_LEFT:
				move = MoveBy::create(0.1f, Vec2(-Director::getInstance()->getVisibleSize().width * 0.5f, 0));
				if (!m_bSpawned)
				{
					m_bSpawned = true;
					// Create new avatar and spawn it to the right
					switch (m_eCurrentState)
					{
					case STATE_SELECT_AVATAR1:
						this->createAvatar(this->m_iP1SelectedAvatar, false);
						break;
					case STATE_SELECT_AVATAR2:
						this->createAvatar(this->m_iP2SelectedAvatar, false);
						break;
					}
				}
				break;
			case ANIMATE_RIGHT:
				move = MoveBy::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, 0));
				if (!m_bSpawned)
				{
					m_bSpawned = true;
					// Create new avatar and spawn it to the left
					switch (m_eCurrentState)
					{
					case STATE_SELECT_AVATAR1:
						this->createAvatar(this->m_iP1SelectedAvatar, true);
						break;
					case STATE_SELECT_AVATAR2:
						this->createAvatar(this->m_iP2SelectedAvatar, true);
						break;
					}
				}
				break;
			}

			animSeq = Sequence::create(move, removeRest, nullptr);
			oldAvatar->runAction(animSeq);
		}
	}

	auto menu = this->getChildByName("Menu");
	if (menu)
	{
		auto avatar = dynamic_cast<Label*>(menu->getChildByName("Avatar No."));
		auto player = dynamic_cast<Label*>(menu->getChildByName("Player"));
		if (avatar)
		{
			std::stringstream ss;
			switch (m_eCurrentState)
			{
			case STATE_SELECT_AVATAR1:
				ss << "Avatar : " << this->m_iP1SelectedAvatar;
				if (player)
				{
					std::stringstream ss1;
					ss1 << "Player 1";
					player->setString(ss1.str());
				}
				break;
			case STATE_SELECT_AVATAR2:
				ss << "Avatar : " << this->m_iP2SelectedAvatar;
				if (player)
				{
					std::stringstream ss1;
					ss1 << "Player 2";
					player->setString(ss1.str());
				}
				break;
			}
			avatar->setString(ss.str());
		}
	}
}

void TutorialModeMenu::createAvatar(int i, bool right)
{
	std::stringstream dirCheck;
	dirCheck << "Avatars/avatar" << i;

	if (FileUtils::getInstance()->isDirectoryExist(dirCheck.str()))
	{
		Sprite * avatar;
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
					switch (m_eCurrentState)
					{
					case STATE_SELECT_AVATAR1:
						ss << "Avatar " << this->m_iP1SelectedAvatar;
						break;
					case STATE_SELECT_AVATAR2:
						ss << "Avatar " << this->m_iP2SelectedAvatar;
						break;
					}
					avatar->setName(ss.str());
					if (right)
					{
						avatar->setPosition(Vec2(-Director::getInstance()->getVisibleSize().width, (Director::getInstance()->getVisibleSize().height * 0.4f)));
						avatar->setScale(GLOBAL_UI_SCALE);
						auto moveTo = MoveTo::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, avatar->getPositionY()));
						auto animSeq2 = Sequence::create(moveTo, nullptr);
						avatar->runAction(animSeq2);
					}
					else
					{
						avatar->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, (Director::getInstance()->getVisibleSize().height * 0.4f)));
						avatar->setScale(GLOBAL_UI_SCALE);
						auto moveTo = MoveTo::create(0.1f, Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, avatar->getPositionY()));
						auto animSeq2 = Sequence::create(moveTo, nullptr);
						avatar->runAction(animSeq2);
					}	
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
		}
		this->addChild(avatar, 1);
	}
	else
	{
		// Placeholder Avatar
	}
}

void TutorialModeMenu::resetAvatar()
{
	std::stringstream ss;
	ss << "Avatar " << m_iP1SelectedAvatar;
	auto cleanAvatar = this->getChildByName(ss.str());
	if (cleanAvatar)
	{
		cleanAvatar->removeFromParent();
		this->createAvatarButtons();
	}
}

void TutorialModeMenu::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void TutorialModeMenu::createListeners()
{
	// Create Keyboard Listener
	auto kListener = EventListenerKeyboard::create();
	kListener->onKeyPressed = CC_CALLBACK_2(TutorialModeMenu::onKeyPressed, this);
	kListener->onKeyReleased = CC_CALLBACK_2(TutorialModeMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kListener, this);

	// Create Mouse Listener
	auto mListener = EventListenerMouse::create();
	mListener->onMouseMove = CC_CALLBACK_1(TutorialModeMenu::onMouseMove, this);
	mListener->onMouseUp = CC_CALLBACK_1(TutorialModeMenu::onMouseUp, this);
	mListener->onMouseDown = CC_CALLBACK_1(TutorialModeMenu::onMouseDown, this);
	mListener->onMouseScroll = CC_CALLBACK_1(TutorialModeMenu::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mListener, this);
}

void TutorialModeMenu::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	switch (keyCode) 
	{
	case EventKeyboard::KeyCode::KEY_0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case EventKeyboard::KeyCode::KEY_1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_AVATAR1:
		case STATE_SELECT_AVATAR2:
		case STATE_SELECT_MAP:
			if (!m_bAnimating)
			{
				this->m_bAnimating = true;
				this->m_eAnimateDir = ANIMATE_RIGHT;
				switch (m_eCurrentState)
				{
				case STATE_SELECT_AVATAR1:
					this->m_iPreviousAvatar = this->m_iP1SelectedAvatar;
					if (m_iP1SelectedAvatar <= m_iTotalAvatars)
					{
						m_iP1SelectedAvatar -= 1;
						if (m_iP1SelectedAvatar <= 0)
						{
							this->m_iP1SelectedAvatar = m_iTotalAvatars;
						}
					}
					break;
				case STATE_SELECT_AVATAR2:
					this->m_iPreviousAvatar = this->m_iP2SelectedAvatar;
					if (m_iP2SelectedAvatar <= m_iTotalAvatars)
					{
						m_iP2SelectedAvatar -= 1;
						if (m_iP2SelectedAvatar <= 0)
						{
							this->m_iP2SelectedAvatar = m_iTotalAvatars;
						}
					}
					break;
				}
			}
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_AVATAR1:
		case STATE_SELECT_AVATAR2:
		case STATE_SELECT_MAP:
			if (!m_bAnimating)
			{
				this->m_bAnimating = true;
				this->m_eAnimateDir = ANIMATE_LEFT;
				switch (m_eCurrentState)
				{
				case STATE_SELECT_AVATAR1:
					this->m_iPreviousAvatar = this->m_iP1SelectedAvatar;
					if (m_iP1SelectedAvatar < m_iTotalAvatars)
					{
						m_iP1SelectedAvatar += 1;
					}
					else
					{
						m_iP1SelectedAvatar = 1;
					}
					break;
				case STATE_SELECT_AVATAR2:
					this->m_iPreviousAvatar = this->m_iP2SelectedAvatar;
					if (m_iP2SelectedAvatar < m_iTotalAvatars)
					{
						m_iP2SelectedAvatar += 1;
					}
					else
					{
						m_iP2SelectedAvatar = 1;
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
		case STATE_SELECT_AVATAR1:
			this->resetAvatar();
			//this->m_eCurrentState = STATE_SELECT_AVATAR2;
			this->changeMenu(STATE_SELECT_MAP);
			break;
		case STATE_SELECT_AVATAR2:
			this->changeMenu(STATE_SELECT_MAP);
			break;
		case STATE_SELECT_MAP:
			auto gameScene = TutorialModeGame::createScene(this->m_iP1SelectedAvatar, this->m_iP2SelectedAvatar, this->m_iSelectedMap, this->m_iLevel);
			Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
		switch (m_eCurrentState)
		{
		case STATE_SELECT_AVATAR1:
			auto gameScene = StageModeMenu::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(1.f, gameScene, Color3B(0, 0, 0)));
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		this->menuCloseCallback(this);
		break;
	}
}

void TutorialModeMenu::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	
}

void TutorialModeMenu::onMouseMove(cocos2d::Event * event)
{

}

void TutorialModeMenu::onMouseUp(cocos2d::Event * event)
{

}

void TutorialModeMenu::onMouseDown(cocos2d::Event * event)
{

}

void TutorialModeMenu::onMouseScroll(cocos2d::Event * event)
{

}