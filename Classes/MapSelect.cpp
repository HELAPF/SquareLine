#include "MapSelect.h"

Scene *MapSelect::createScene() {
	auto scene = Scene::create();
	auto layer = MapSelect::create();
	scene->addChild(layer);

	return scene;
}

bool MapSelect::init() {
	if (!Layer::init()) { return false; }

	background = Sprite::create("Background/menu_classic.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background);

	for (int i = 0; i < 5; i++) {
		std::stringstream temp, check_temp;
		temp << "map/icon/" << i << ".png";

		icon[i] = Sprite::create(temp.str());
		icon[i]->setAnchorPoint(Vec2(0.5, 0.5));
		switch (i) {
		case 0:
			icon[i]->setPosition(visibleSize.width * 1 / 6, visibleSize.height / 2 + 100);
			break;
		case 1:
			icon[i]->setPosition(visibleSize.width * 1 / 2, visibleSize.height / 2 + 100);
			break;
		case 2:
			icon[i]->setPosition(visibleSize.width * 5 / 6, visibleSize.height / 2 + 100);
			break;
		case 3:
			icon[i]->setPosition(visibleSize.width * 1 / 4, visibleSize.height / 2 - 200);
			break;
		case 4:
			icon[i]->setPosition(visibleSize.width * 3 / 4, visibleSize.height / 2 - 200);
			break;
		default:
			break;
		}

		check_temp << "mapProgress" << i;
		std::string string_temp = check_temp.str();
		const char *save_Check_temp = string_temp.c_str();

		if (UserDefault::getInstance()->getBoolForKey(save_Check_temp) == false) {
			icon[i]->setColor(Color3B::BLACK);
			map_Exist[i] = UserDefault::getInstance()->getBoolForKey(save_Check_temp);
		}
		else {
			map_Exist[i] = UserDefault::getInstance()->getBoolForKey(save_Check_temp);
		}
		this->addChild(icon[i]);
	}

	select = 0;

	cursor = Sprite::create("map/icon/cursor.png");
	cursor->setPosition(icon[select]->getPosition());
	this->addChild(cursor);

	SimpleAudioEngine::sharedEngine()->preloadEffect(SELECT);
	SimpleAudioEngine::sharedEngine()->preloadEffect(DENY);

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(MapSelect::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(MapSelect::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
}

void MapSelect::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (select <= 2) { break; }
		else {
			select -= 3;
			cursor->setPosition(icon[select]->getPosition());
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (select >= 3) { break; }
		else {
			if (select == 2) { select = 4; }
			else { select += 3; }
			cursor->setPosition(icon[select]->getPosition());
		}
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (select == 0 || select == 3) { break; }
		else {
			select--;
			cursor->setPosition(icon[select]->getPosition());
		}
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (select == 2 || select == 4) { break; }
		else {
			select++;
			cursor->setPosition(icon[select]->getPosition());
		}
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (map_Exist[select] == false) {
			SimpleAudioEngine::sharedEngine()->playEffect(DENY, false);
		}
		else {
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			SimpleAudioEngine::sharedEngine()->playEffect(SELECT, false);
			UserDefault::getInstance()->setIntegerForKey("mapSelect", select);
			Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0, Classic_Ingame::createScene()));
		}
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		Director::getInstance()->replaceScene(TransitionFadeBL::create(2.0, AvatarSelect::createScene()));
		break;

	default:
		break;
	}
}

void MapSelect::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event * event) {

}
