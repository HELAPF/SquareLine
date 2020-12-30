#include "AvatarSelect.h"

Scene *AvatarSelect::createScene() {
	auto scene = Scene::create();
	auto layer = AvatarSelect::create();
	scene->addChild(layer);

	return scene;
}

bool AvatarSelect::init() {
	if (!Layer::init()) { return false; }

	background = Sprite::create("Background/menu_classic.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background, 0);

	message = Label::createWithTTF("Select Your Avatar!", "fonts/power_pixel-7.ttf", 40);
	message->setPosition(visibleSize.width / 2, 650);
	message->setColor(Color3B::BLACK);
	this->addChild(message, 1);

	tile = Sprite::create("UI/settings/tile.png");
	tile->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(tile, 2);

	for (int i = 0; i < 12; i++) {
		std::stringstream streamtemp;
		streamtemp << "avatarProgress" << i;

		std::string stringtemp = streamtemp.str();
		const char *temp = stringtemp.c_str();

		std::stringstream dir;
		if (UserDefault::getInstance()->getBoolForKey(temp) == 1) {
			dir << "Avatar/select/" << i << ".png";
			std::string ttemp = dir.str();
			avatarList[i] = Sprite::create(ttemp);
			avatarList[i]->setAnchorPoint(Vec2(0, 1));
			avatarList[i]->setPosition(tile->getPositionX() + avatarList[i]->getContentSize().width * ((i % 6) - 3), tile->getPositionY() + avatarList[i]->getContentSize().height * (1 - (i / 6)));
			this->addChild(avatarList[i], 1);

			avatar_Exist[i] = UserDefault::getInstance()->getBoolForKey(temp);
			dir.str("");
		}
		else {
			dir << "Avatar/select/silhouette/" << i << ".png";
			std::string ttemp = dir.str();
			avatarList[i] = Sprite::create(ttemp);
			avatarList[i]->setAnchorPoint(Vec2(0, 1));
			avatarList[i]->setPosition(tile->getPositionX() + avatarList[i]->getContentSize().width * ((i % 6) - 3), tile->getPositionY() + avatarList[i]->getContentSize().height * (1 - (i / 6)));
			this->addChild(avatarList[i], 1);

			avatar_Exist[i] = UserDefault::getInstance()->getBoolForKey(temp);
			if (i == 11) { avatar_Exist[i] = true; }
			dir.str("");
		}
	}

	p1_Cursor = Sprite::create("Avatar/select/1p_cursor.png");
	p1_Select_Bubble = Sprite::create("Avatar/select/1p_selected.png");

	p1_Cursor->setAnchorPoint(Vec2(0, 1));
	p1_Select_Bubble->setAnchorPoint(Vec2(0, 1));
	p1_Select = 0;
	p1_Cursor->setPosition(avatarList[p1_Select]->getPosition());
	p1_Select_Bubble->setVisible(false);
	this->addChild(p1_Cursor, 2);
	this->addChild(p1_Select_Bubble, 2);

	is_p1_Selected = false;

	p2_Cursor = Sprite::create("Avatar/select/2p_cursor.png");
	p2_Select_Bubble = Sprite::create("Avatar/select/2p_selected.png");

	p2_Cursor->setAnchorPoint(Vec2(0, 1));
	p2_Select_Bubble->setAnchorPoint(Vec2(0, 1));
	p2_Select = 1;
	p2_Cursor->setPosition(avatarList[p2_Select]->getPosition());
	p2_Select_Bubble->setVisible(false);
	this->addChild(p2_Cursor, 2);
	this->addChild(p2_Select_Bubble, 2);

	is_p2_Selected = false;

	SimpleAudioEngine::sharedEngine()->preloadEffect(SELECT);
	SimpleAudioEngine::sharedEngine()->preloadEffect(DENY);

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(AvatarSelect::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(AvatarSelect::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
}

void AvatarSelect::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
		////////////////
		// 1p Move
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (is_p1_Selected == false) {
			if (p1_Select >= 6 && p1_Select <= 11) {
				p1_Select -= 6;
				p1_Cursor->setPosition(avatarList[p1_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (is_p1_Selected == false) {
			if (p1_Select >= 0 && p1_Select <= 5) {
				p1_Select += 6;
				p1_Cursor->setPosition(avatarList[p1_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (is_p1_Selected == false) {
			if (p1_Select == 0 || p1_Select == 6) { break; }
			else {
				p1_Select--;
				p1_Cursor->setPosition(avatarList[p1_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (is_p1_Selected == false) {
			if (p1_Select == 5 || p1_Select == 11) { break; }
			else {
				p1_Select++;
				p1_Cursor->setPosition(avatarList[p1_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (is_p1_Selected == false) {
			if (avatar_Exist[p1_Select] == true) {
				is_p1_Selected = true;
				SimpleAudioEngine::sharedEngine()->playEffect(SELECT);
				p1_Select_Bubble->setVisible(true);
				p1_Select_Bubble->setPosition(avatarList[p1_Select]->getPosition());
				UserDefault::getInstance()->setIntegerForKey("1pAvatarSelect", p1_Select);
				AvatarSelect::allPickCheck();
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(DENY);
			}
		}
		break;

		////////////////
		// 2p Move
	case EventKeyboard::KeyCode::KEY_W:
		if (is_p2_Selected == false) {
			if (p2_Select >= 6 && p2_Select <= 11) {
				p2_Select -= 6;
				p2_Cursor->setPosition(avatarList[p2_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
		if (is_p2_Selected == false) {
			if (p2_Select >= 0 && p2_Select <= 5) {
				p2_Select += 6;
				p2_Cursor->setPosition(avatarList[p2_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_A:
		if (is_p2_Selected == false) {
			if (p2_Select == 0 || p2_Select == 6) { break; }
			else {
				p2_Select--;
				p2_Cursor->setPosition(avatarList[p2_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (is_p2_Selected == false) {
			if (p2_Select == 5 || p2_Select == 11) { break; }
			else {
				p2_Select++;
				p2_Cursor->setPosition(avatarList[p2_Select]->getPosition());
				break;
			}
		}
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_SHIFT:
		if (is_p2_Selected == false) {
			if (avatar_Exist[p2_Select] == true) {
				is_p2_Selected = true;
				p2_Select_Bubble->setPosition(avatarList[p2_Select]->getPositionX(), avatarList[p2_Select]->getPositionY() - 206);
				SimpleAudioEngine::sharedEngine()->playEffect(SELECT);
				p2_Select_Bubble->setVisible(true);
				UserDefault::getInstance()->setIntegerForKey("2pAvatarSelect", p2_Select);
				AvatarSelect::allPickCheck();
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(DENY);
			}
		}
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		if (is_p2_Selected == true) {
			is_p2_Selected = false;
			p2_Select_Bubble->setVisible(false);
			break;
		}
		if (is_p1_Selected == true) {
			is_p1_Selected = false;
			p1_Select_Bubble->setVisible(false);
			break;
		}
		if (is_p1_Selected == false && is_p2_Selected == false) {
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0, Mode::createScene()));
			break;
		}
		break;

	default:
		break;
	}
}
void AvatarSelect::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {

}

void AvatarSelect::allPickCheck() {
	if (is_p1_Selected == true && is_p2_Selected == true) {
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0, MapSelect::createScene()));
	}
}