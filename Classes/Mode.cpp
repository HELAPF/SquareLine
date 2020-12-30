#include "Mode.h"
#include "Start.h"

#include "StageModeMenu.h"

USING_NS_CC;

Scene *Mode::createScene() {
	auto scene = Scene::create();
	auto layer = Mode::create();
	scene->addChild(layer);

	return scene;
}

bool Mode::init() {
	if (!Layer::init()) { return false; }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("Background/menu_classic.png");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background);

	buttonLeft = Sprite::create("button_mode/press.png");
	buttonLeft->setAnchorPoint(Vec2(0.5, 0.5));
	buttonLeft->setRotation(-90);
	buttonLeft->setPosition(Vec2(visibleSize.width / 8 - origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(buttonLeft);

	buttonRight = Sprite::create("button_mode/press.png");
	buttonRight->setAnchorPoint(Vec2(0.5, 0.5));
	buttonRight->setRotation(90);
	buttonRight->setPosition(Vec2(visibleSize.width * 7 / 8 - origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(buttonRight);

	classicMode = Sprite::create("button_mode/classic_ns.png");
	classicMode->setPosition(Vec2(visibleSize.width * 3 / 8, visibleSize.height / 2));
	this->addChild(classicMode);

	stageMode = Sprite::create("button_mode/stage_ns.png");
	stageMode->setPosition(Vec2(visibleSize.width * 5 / 8, visibleSize.height / 2));
	this->addChild(stageMode);

	//sound preload
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BGM);
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() == true) {

	}
	else {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(BGM, true);
	}

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(Mode::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(Mode::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	this->schedule(schedule_selector(Mode::SelectedMode));
}

void Mode::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		buttonLeft->setTexture("button_mode/Press_s.png");
		switch (nowSelected) {
		case CLASSIC:
			nowSelected = STAGE;
			break;
		default:
			nowSelected--;
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		buttonRight->setTexture("button_mode/Press_s.png");
		switch (nowSelected) {
		case STAGE:
			nowSelected = CLASSIC;
			break;
		default:
			nowSelected++;
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_ENTER:
		switch (nowSelected) {
		case CLASSIC:
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0, AvatarSelect::createScene()));
			break;
		case STAGE:
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0, StageModeMenu::createScene()));
			break;
		default:
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		ChangeScene();
		break;

	default:
		break;
	}
}
void Mode::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		buttonLeft->setTexture("button_mode/Press.png");
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		buttonRight->setTexture("button_mode/Press.png");
		break;
	}
}

void Mode::SelectedMode(float delta) {
	switch (nowSelected) {
	case CLASSIC:
		classicMode->setTexture("button_mode/classic_s.png");
		stageMode->setTexture("button_mode/stage_ns.png");
		break;
	case STAGE:
		classicMode->setTexture("button_mode/classic_ns.png");
		stageMode->setTexture("button_mode/stage_s.png");
		break;
	default:
		break;
	}
}

void Mode::ChangeScene() {
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Scene *ChangeScene = TransitionFadeTR::create(2.0, Start::createScene());
	Director::getInstance()->replaceScene(ChangeScene);
}