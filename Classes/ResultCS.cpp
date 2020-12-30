#include "ResultCS.h"

Scene *ResultCS::createScene() {
	auto scene = Scene::create();
	auto layer = ResultCS::create();
	scene->addChild(layer);

	return scene;
}

bool ResultCS::init() {
	if (!Layer::init()) { return false; }

	background = Sprite::create("Background/menu.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background, 1);

	resultCube = Sprite::create("UI/result/PVP/cube_result.png");
	resultCube->setPosition(Vec2(visibleSize.width / 2, 1000));
	resultCube->setTag(0);

	winnerText = Label::createWithTTF("", "fonts/power_pixel-7.ttf", 40);
	scoreGapText = Label::createWithTTF("", "fonts/power_pixel-7.ttf", 40);
	remainTimeText = Label::createWithTTF("", "fonts/power_pixel-7.ttf", 40);

	winner = UserDefault::getInstance()->getStringForKey("TempWinner");
	scoreGap = UserDefault::getInstance()->getIntegerForKey("TempScoreGap");
	remainTime = UserDefault::getInstance()->getIntegerForKey("TempRemainTime");

	const char *temp = winner.c_str();
	winnerText->setString(StringUtils::format(temp));
	winnerText->setColor(Color3B::BLACK);
	winnerText->setPosition(Vec2(640, resultCube->getPositionY() + 80));
	winnerText->setRotation3D(Vec3(309.6, 0, 0));
	winnerText->setTag(1);

	scoreGapText->setString(StringUtils::format("%d", scoreGap));
	scoreGapText->setColor(Color3B::BLACK);
	scoreGapText->setPosition(Vec2(540, resultCube->getPositionY() - 140));
	scoreGapText->setRotation3D(Vec3(-10, 0, 30));
	scoreGapText->setTag(2);

	remainTimeText->setString(StringUtils::format("%d", remainTime));
	remainTimeText->setColor(Color3B::BLACK);
	remainTimeText->setPosition(Vec2(740, resultCube->getPositionY() - 140));
	remainTimeText->setRotation3D(Vec3(10, 0, 330));
	remainTimeText->setTag(3);

	this->addChild(resultCube, 1);
	this->addChild(winnerText, 2);
	this->addChild(scoreGapText, 2);
	this->addChild(remainTimeText, 2);

	selected = true;

	this->scheduleOnce(schedule_selector(ResultCS::CubeDrop), 1.0f);
}

void ResultCS::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (selected == true) {
			selected = false;
		}
		else {
			selected = true;
		}
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (selected == false) {
			selected = true;
		}
		else {
			selected = false;
		}
		break;

	case EventKeyboard::KeyCode::KEY_ENTER:
		if (selected == true) {
			this->unscheduleAllSelectors();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, Classic_Ingame::createScene()));
		}
		else {
			this->unscheduleAllSelectors();
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, Mode::createScene()));
		}
		break;

	default:
		break;
	}
}

void ResultCS::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {

}

void ResultCS::CubeDrop(float delta) {
	resultCube->runAction(MoveBy::create(1.0f, Vec2(0, -640)));
	winnerText->runAction(MoveBy::create(1.0f, Vec2(0, -630)));
	scoreGapText->runAction(MoveBy::create(1.0f, Vec2(0, -650)));
	remainTimeText->runAction(MoveBy::create(1.0f, Vec2(0, -650)));

	this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(CC_CALLBACK_0(ResultCS::ToggleToMenu, this)), NULL));
}

void ResultCS::ToggleToMenu() {
	resultCube->setTexture("UI/result/PVP/End/Normal_classic_mode.png");
	winnerText->runAction(FadeOut::create(0.5f));
	scoreGapText->runAction(FadeOut::create(0.5f));
	remainTimeText->runAction(FadeOut::create(0.5f));

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(ResultCS::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(ResultCS::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	this->schedule(schedule_selector(ResultCS::update));
}

void ResultCS::update(float delta) {
	if (selected == true) {
		resultCube->setTexture("UI/result/PVP/End/Normal_classic_mode_r.png");
	}
	else {
		resultCube->setTexture("UI/result/PVP/End/Normal_classic_mode_n.png");
	}
}