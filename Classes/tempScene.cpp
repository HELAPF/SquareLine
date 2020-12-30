#include "tempScene.h"

Scene *tempScene::createScene() {
	auto scene = Scene::create();
	auto layer = tempScene::create();
	scene->addChild(layer);

	return scene;
}

bool tempScene::init() {
	if (!Layer::init()) { return false; }

	background = Sprite::create("UI/result/Title.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background, 0);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			tile[i][j] = Sprite::create(BLACKTILE);
			tile[i][j]->setAnchorPoint(Vec2(0, 0));
			tile[i][j]->setPosition(Vec2(i * 128, j * 128));
			this->addChild(tile[i][j], 1);
		}
	}

	this->scheduleOnce(schedule_selector(tempScene::changeScene), 3.0f);
}

void tempScene::changeScene(float delta) {
	Director::getInstance()->replaceScene(TransitionFadeDown::create(1.0f, ResultCS::createScene()));
}