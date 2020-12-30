#include "Logo_Scene.h"

Scene *Logo_Scene::createScene() {
	auto scene = Scene::create();
	auto layer = Logo_Scene::create();
	scene->addChild(layer);
	return scene;
}

bool Logo_Scene::init() {
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	background = Sprite::create("logo_background.png");
	logo_sprite = Sprite::create("logo.jpg");

	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	logo_sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(background, 0);
	this->addChild(logo_sprite, 1);

	this->scheduleOnce(schedule_selector(Logo_Scene::SceneChange), 3.0f);
}

void Logo_Scene::SceneChange(float delta) {
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Start::createScene()));
}