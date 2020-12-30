#include "Start.h"
#include "Mode.h"

USING_NS_CC;

Scene *Start::createScene() {
	auto scene = Scene::create();
	auto layer = Start::create();
	scene->addChild(layer);
	return scene;
}

bool Start::init() {
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	///////////////
	// UI Initialize

	auto background = Sprite::create("Background/menu_classic.png");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	Sprite *title = Sprite::create("title.png");
	title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 150));
	this->addChild(title, 1);

	startButton = Label::createWithTTF("Game Start", "fonts/power_pixel-7.ttf", 40);
	startButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));
	startButton->setColor(Color3B(0, 0, 0));
	this->addChild(startButton, 1);

	exitButton = Label::createWithTTF("Exit", "fonts/power_pixel-7.ttf", 40);
	exitButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 150));
	exitButton->setColor(Color3B(0, 0, 0));
	this->addChild(exitButton, 1);

	///////////////
	// Sound Preload
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_INCORRECT);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BGM);
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() || SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {

	}
	else {
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(BGM, true);
	}

	///////////////
	// Data preset
	UserDefault::getInstance()->setBoolForKey("avatarProgress0", true);
	UserDefault::getInstance()->setBoolForKey("mapProgress0", true);

	///////////////
	// Keyboard Listener Initialize

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(Start::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(Start::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	this->schedule(schedule_selector(Start::SelectedButton));
}

void Start::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		switch (nowSelected) {
		case START:
			nowSelected = EXIT;
			break;
		default:
			nowSelected--;
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		switch (nowSelected) {
		case EXIT:
			nowSelected = START;
			break;
		default:
			nowSelected++;
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_ENTER:
		ChangeScene(nowSelected);
		break;

	default:
		break;
	}
}

void Start::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {

}

void Start::SelectedButton(float delta) {
	switch (nowSelected) {
	case START:
		startButton->setColor(Color3B(255, 255, 0));
		exitButton->setColor(Color3B(0, 0, 0));
		break;
	case EXIT:
		startButton->setColor(Color3B(0, 0, 0));
		exitButton->setColor(Color3B(255, 255, 0));
		break;
	default:
		break;
	}
}

void Start::ChangeScene(int type) {
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Scene *temp;

	switch (type) {
	case START:
		temp = TransitionFadeTR::create(2.0, Mode::createScene());
		Director::getInstance()->replaceScene(temp);
		break;
	case EXIT:
		Director::getInstance()->end();
		break;
	}
}
