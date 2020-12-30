#include "Classic_Ingame.h"

mapInit* mapInit::p_instance;

Scene *Classic_Ingame::createScene() {
	auto scene = Scene::create();
	auto layer = Classic_Ingame::create();
	scene->addChild(layer);

	return scene;
}

bool Classic_Ingame::init() {
	if (!Layer::init()) { return false; }

	//background initialize
	background = Sprite::create("Background/classic.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background, 0);

	board_Background = Sprite::create("UI/icon/map_resized.png");
	board_Background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(board_Background, 0);

	//mapinit singletone pointer
	mapInit *instance = nullptr;
	instance = mapInit::GetInstance();
	//maptile initialize
	int select = UserDefault::getInstance()->getIntegerForKey("mapSelect");
	instance->Initialize(select);
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			tile[i][j] = Sprite::create();
		}
	}
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			switch (instance->GetTile(i, j)) {
			case 1:
				tile[i][j]->setTexture(FILE1);
				break;
			case 2:
				tile[i][j]->setTexture(FILE2);
				break;
			case 3:
				tile[i][j]->setTexture(FILE3);
				break;
			case 4:
				tile[i][j]->setTexture(FILE4);
				break;
			case 5:
				tile[i][j]->setTexture(FILE5);
				break;
			case 6:
				tile[i][j]->setTexture(FILE6);
				break;
			case 7:
				tile[i][j]->setTexture(FILE7);
				break;
			case 8:
				tile[i][j]->setTexture(FILE8);
				break;
			case 9:
				tile[i][j]->setTexture(FILE9);
				break;
			default:
				break;
			}
			tile[j][i]->setAnchorPoint(Vec2(0, 0));
			tile[j][i]->setPosition(GetTile(i, j));
			this->addChild(tile[j][i], 1);
			if (instance->GetTile(i, j) != 9) {
				map_Status[i][j] = NORMAL;
			}
			else {
				map_Status[i][j] = BLOCKED;
			}

			p1_select[j][i] = Sprite::create();
			p1_select[j][i]->setTexture("map/1p_cursor.png");
			p1_select[j][i]->setAnchorPoint(Vec2(0, 0));
			p1_select[j][i]->setPosition(GetTile(i, j));
			p1_select[j][i]->setVisible(false);
			this->addChild(p1_select[j][i], 2);

			p2_select[j][i] = Sprite::create();
			p2_select[j][i]->setTexture("map/2p_cursor.png");
			p2_select[j][i]->setAnchorPoint(Vec2(0, 0));
			p2_select[j][i]->setPosition(GetTile(i, j));
			p2_select[j][i]->setVisible(false);
			this->addChild(p2_select[j][i], 2);
		}
	}
	instance->ClearData();

	//cursor initalize
	p1_cursor = Sprite::create("map/1p_cursor.png");
	p2_cursor = Sprite::create("map/2p_cursor.png");

	p1_cursor->setAnchorPoint(Vec2(0, 0));
	p2_cursor->setAnchorPoint(Vec2(0, 0));

	p1_cursor->setPosition(GetTile(5, 5));
	p2_cursor->setPosition(GetTile(6, 6));

	this->addChild(p1_cursor, 2);
	this->addChild(p2_cursor, 2);

	p1_cursor->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5f), FadeIn::create(0.5f), NULL)));
	p2_cursor->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5f), FadeIn::create(0.5f), NULL)));

	p1_isMoved = FALSE;
	p2_isMoved = FALSE;

	p1_Movement.clear();
	p2_Movement.clear();
	Classic_Ingame::GetCursorCoord();

	map_Status[p1_Coord.y][p1_Coord.x] = P1_SELECTED;
	map_Status[p2_Coord.y][p2_Coord.x] = P2_SELECTED;

	p1_Movement.push_back(p1_Coord);
	p2_Movement.push_back(p2_Coord);

	///////////////
	// UI initialize

	//UI initialize
	p1_ScoreUI = Sprite::create("UI/icon/score_display.png");
	p1_SpeechBubble = Sprite::create("UI/bubble/speech_bubble.png");
	p1_Avatar = Sprite::create();
	p1_Name = Sprite::create("UI/icon/1p_nick.png");

	p2_ScoreUI = Sprite::create("UI/icon/score_display.png");
	p2_SpeechBubble = Sprite::create("UI/bubble/speech_bubble.png");
	p2_Avatar = Sprite::create();
	p2_Name = Sprite::create("UI/icon/2p_nick.png");

	missionUI = Sprite::create("UI/icon/mission.png");
	timeBackground = Sprite::create("UI/icon/remain_time.png");
	timeUI = Sprite::create("UI/icon/time.png");

	gameoverBox = Sprite::create("UI/result/PVP/cube_classic.png");

	count_down = Label::createWithCharMap("fonts/no_01.png", 33, 45, 48);
	mission = LabelTTF::create(StringUtils::format("Mission Range : %d / Mission Pair : %d", mission_Range, mission_Pair), "Arial", 20);
	score1 = Label::createWithCharMap("fonts/no_01.png", 33, 45, 48);
	score2 = Label::createWithCharMap("fonts/no_01.png", 33, 45, 48);

	if (UserDefault::getInstance()->getIntegerForKey("1pAvatarSelect") == 11) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, 11);

		while (1) {
			std::stringstream streamtemp;
			int num = dist(gen);
			streamtemp << "avatarProgress" << num;
			std::string temp = streamtemp.str();

			if (UserDefault::getInstance()->getBoolForKey(temp.c_str()) == true) {
				p1_AvatarSelect = num;
				break;
			}
		}
	}
	else {
		p1_AvatarSelect = UserDefault::getInstance()->getIntegerForKey("1pAvatarSelect");
	}

	if (UserDefault::getInstance()->getIntegerForKey("2pAvatarSelect") == 11) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, 11);

		while (1) {
			std::stringstream streamtemp;
			int num = dist(gen);
			streamtemp << "avatarProgress" << num;
			std::string temp = streamtemp.str();

			if (UserDefault::getInstance()->getBoolForKey(temp.c_str()) == true) {
				p2_AvatarSelect = num;
				break;
			}
		}
	}
	else {
		p2_AvatarSelect = UserDefault::getInstance()->getIntegerForKey("2pAvatarSelect");
	}

	Classic_Ingame::SpriteAnimationInitialize(1, p1_AvatarSelect);
	p1_ScoreUI->setPosition(Vec2(1110, 720 - 150 + 50));
	p1_SpeechBubble->setPosition(Vec2(1110, 720 - 300 + 50));
	p1_Avatar->setPosition(Vec2(1110, 720 - 525 + 50));
	p1_Name->setPosition(Vec2(1110, 720 - 682 + 50));
	this->addChild(p1_ScoreUI, 1);
	this->addChild(p1_SpeechBubble, 1);
	this->addChild(p1_Avatar, 1);
	this->addChild(p1_Name, 1);

	Classic_Ingame::SpriteAnimationInitialize(2, p2_AvatarSelect);
	p2_ScoreUI->setPosition(Vec2(170, 720 - 150 + 50));
	p2_SpeechBubble->setPosition(Vec2(170, 720 - 300 + 50));
	p2_Avatar->setPosition(Vec2(170, 720 - 525 + 50));
	p2_Name->setPosition(Vec2(170, 720 - 682 + 50));
	this->addChild(p2_ScoreUI, 1);
	this->addChild(p2_SpeechBubble, 1);
	this->addChild(p2_Avatar, 1);
	this->addChild(p2_Name, 1);

	missionUI->setPosition(Vec2(640, 720 - 36));
	this->addChild(missionUI, 1);
	timeUI->setAnchorPoint(Vec2(1, 0.5));
	timeUI->setRotation(270.0f);
	timeUI->setPosition(Vec2(background->getPositionX(), 720 - 690));
	timeBackground->setRotation(270.0f);
	timeBackground->setPosition(timeUI->getPosition());
	timeUI_OriginMinX = timeUI->getTextureRect().getMinX();
	timeUI_OriginMaxX = timeUI->getTextureRect().getMaxX();
	timeUI_OriginMinY = timeUI->getTextureRect().getMinY();
	timeUI_OriginMaxY = timeUI->getTextureRect().getMaxY();
	this->addChild(timeUI, 2);
	this->addChild(timeBackground, 1);

	gameoverBox->setPosition(Vec2(640, 800));
	gameoverBox->setVisible(false);
	this->addChild(gameoverBox, 3);

	//mission initilalize
	mission_Range = 6;
	mission_Pair = 7;
	mission->setString(StringUtils::format("Mission Range : 3 - %d / Mission Pair : %d", mission_Range, mission_Pair));
	mission->setPosition(Point(missionUI->getPosition().x, missionUI->getPosition().y));
	mission->setColor(Color3B::BLACK);
	this->addChild(mission, 2);

	p1_Score = 0;
	p2_Score = 0;

	p1_DisplayScore = p1_Score;
	p2_DisplayScore = p2_Score;

	score1->setAnchorPoint(Vec2(1, 0.5));
	score1->setString(StringUtils::format("%d", p1_DisplayScore));
	score1->setPosition(Vec2(p1_ScoreUI->getPosition().x + p1_ScoreUI->getContentSize().width / 3, p1_ScoreUI->getPosition().y));
	this->addChild(score1, 2);

	score2->setAnchorPoint(Vec2(1, 0.5));
	score2->setString(StringUtils::format("%d", p2_DisplayScore));
	score2->setPosition(Vec2(p2_ScoreUI->getPosition().x + p2_ScoreUI->getContentSize().width / 3, p2_ScoreUI->getPosition().y));
	this->addChild(score2, 2);

	//playtime initialize
	remainTime = 60;

	//sound, FX initialize
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_MOVE);
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_CORRECT);
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_INCORRECT);
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_GAMEOVER);
	SimpleAudioEngine::sharedEngine()->preloadEffect(FX_BREAK);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("FX/classic_theme.wav");
	if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {

	}
	else {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("FX/classic_theme.wav", true);
	}

	countDown = 5;
	count_down->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	count_down->setString(StringUtils::format("%d", countDown));
	this->addChild(count_down, 1);
	this->schedule(schedule_selector(Classic_Ingame::PlayCountDown), 1.0f);
}

void Classic_Ingame::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	MoveBy *moveUp = MoveBy::create(0, Point(0, 50));
	MoveBy *moveDown = MoveBy::create(0, Point(0, -50));
	MoveBy *moveLeft = MoveBy::create(0, Point(-50, 0));
	MoveBy *moveRight = MoveBy::create(0, Point(50, 0));

	Blink *blink = Blink::create(0.5, 2);

	TintTo *p1_color = TintTo::create(0, 0, 0, 0);
	FadeTo *halfInvisible = FadeTo::create(0, 128);
	Spawn *p1_Select = Spawn::create(p1_color, halfInvisible, NULL);
	switch (keyCode) {

		//1p move
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (p1_isMoved == TRUE) { break; }
		else {
			p1_isMoved = TRUE;
			if (p1_cursor->getPositionY() + 50 > GetTileElement(1, 1, 2)) {
				break;
			}
			else if (map_Status[p1_Coord.y - 1][p1_Coord.x] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p1_Coord.y - 1][p1_Coord.x] = P1_SELECTED;
				if (map_Status[p1_Coord.y][p1_Coord.x] != P1_OWNED) {
					p1_select[p1_Coord.y][p1_Coord.x]->setVisible(true);
				}
				temp_Coord = p1_Coord;
				temp_Coord.y--;
				p1_Movement.push_back(temp_Coord);
				p1_cursor->runAction(moveUp);
				Classic_Ingame::MissionFailedCheck(1);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (p1_isMoved == TRUE) { break; }
		else {
			p1_isMoved = TRUE;
			if (p1_cursor->getPositionY() - 50 < GetTileElement(1, 12, 2)) {
				break;
			}
			else if (map_Status[p1_Coord.y + 1][p1_Coord.x] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p1_Coord.y + 1][p1_Coord.x] = P1_SELECTED;
				if (map_Status[p1_Coord.y][p1_Coord.x] != P1_OWNED) {
					p1_select[p1_Coord.y][p1_Coord.x]->setVisible(true);
				}
				temp_Coord = p1_Coord;
				temp_Coord.y++;
				p1_Movement.push_back(temp_Coord);
				p1_cursor->runAction(moveDown);
				Classic_Ingame::MissionFailedCheck(1);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (p1_isMoved == TRUE) { break; }
		else {
			p1_isMoved = TRUE;
			if (p1_cursor->getPositionX() - 50 < GetTileElement(1, 1, 1)) {
				break;
			}
			else if (map_Status[p1_Coord.y][p1_Coord.x - 1] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p1_Coord.y][p1_Coord.x - 1] = P1_SELECTED;
				if (map_Status[p1_Coord.y][p1_Coord.x] != P1_OWNED) {
					p1_select[p1_Coord.y][p1_Coord.x]->setVisible(true);
				}
				temp_Coord = p1_Coord;
				temp_Coord.x--;
				p1_Movement.push_back(temp_Coord);
				p1_cursor->runAction(moveLeft);
				Classic_Ingame::MissionFailedCheck(1);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (p1_isMoved == TRUE) { break; }
		else {
			p1_isMoved = TRUE;
			if (p1_cursor->getPositionX() + 50 > GetTileElement(12, 1, 1)) {
				break;
			}
			else if (map_Status[p1_Coord.y][p1_Coord.x + 1] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p1_Coord.y][p1_Coord.x + 1] = P1_SELECTED;
				if (map_Status[p1_Coord.y][p1_Coord.x] != P1_OWNED) {
					p1_select[p1_Coord.y][p1_Coord.x]->setVisible(true);
				}
				temp_Coord = p1_Coord;
				temp_Coord.x++;
				p1_Movement.push_back(temp_Coord);
				p1_cursor->runAction(moveRight);
				Classic_Ingame::MissionFailedCheck(1);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (p1_Movement.size() >= 3 && p1_Movement.size() <= 6) {
			Classic_Ingame::MissionClearCheck(1);
			break;
		}
		else if (p1_Movement.size() != 0) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_INCORRECT, false);
			p1_cursor->setPosition(GetTile(p1_Movement.front().x, p1_Movement.front().y));
			TileDataClear(1);
			p1_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p1_Movement.push_back(p1_Coord);
			map_Status[p1_Coord.y][p1_Coord.x] = P1_SELECTED;
			SetPlayerSpeechBubble(1, p1_AvatarSelect, FAILED);
			break;
		}
		else { break; }

		//2p move
	case EventKeyboard::KeyCode::KEY_W:
		if (p2_isMoved == TRUE) { break; }
		else {
			p2_isMoved = TRUE;
			if (p2_cursor->getPositionY() + 50 > GetTileElement(1, 1, 2)) {
				break;
			}
			else if (map_Status[p2_Coord.y - 1][p2_Coord.x] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p2_Coord.y - 1][p2_Coord.x] = P2_SELECTED;
				if (map_Status[p2_Coord.y][p2_Coord.x] != P2_OWNED) {
					p2_select[p2_Coord.y][p2_Coord.x]->setVisible(true);
				}
				temp_Coord = p2_Coord;
				temp_Coord.y--;
				p2_Movement.push_back(temp_Coord);
				p2_cursor->runAction(moveUp);
				Classic_Ingame::MissionFailedCheck(2);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_S:
		if (p2_isMoved == TRUE) { break; }
		else {
			p2_isMoved = TRUE;
			if (p2_cursor->getPositionY() - 50 < GetTileElement(1, 12, 2)) {
				break;
			}
			else if (map_Status[p2_Coord.y + 1][p2_Coord.x] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p2_Coord.y + 1][p2_Coord.x] = P2_SELECTED;
				if (map_Status[p2_Coord.y][p2_Coord.x] != P2_OWNED) {
					p2_select[p2_Coord.y][p2_Coord.x]->setVisible(true);
				}
				temp_Coord = p2_Coord;
				temp_Coord.y++;
				p2_Movement.push_back(temp_Coord);
				p2_cursor->runAction(moveDown);
				Classic_Ingame::MissionFailedCheck(2);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_A:
		if (p2_isMoved == TRUE) { break; }
		else {
			p2_isMoved = TRUE;
			if (p2_cursor->getPositionX() - 50 < GetTileElement(1, 1, 1)) {
				break;
			}
			else if (map_Status[p2_Coord.y][p2_Coord.x - 1] != NORMAL) {
				break;
			}
			else {
				map_Status[p2_Coord.y][p2_Coord.x - 1] = P2_SELECTED;
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				if (map_Status[p2_Coord.y][p2_Coord.x] != P2_OWNED) {
					p2_select[p2_Coord.y][p2_Coord.x]->setVisible(true);
				}
				temp_Coord = p2_Coord;
				temp_Coord.x--;
				p2_Movement.push_back(temp_Coord);
				p2_cursor->runAction(moveLeft);
				Classic_Ingame::MissionFailedCheck(2);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_D:
		if (p2_isMoved == TRUE) { break; }
		else {
			p2_isMoved = TRUE;
			if (p2_cursor->getPositionX() + 50 > GetTileElement(12, 1, 1)) {
				break;
			}
			else if (map_Status[p2_Coord.y][p2_Coord.x + 1] != NORMAL) {
				break;
			}
			else {
				SimpleAudioEngine::sharedEngine()->playEffect(FX_MOVE, false);
				map_Status[p2_Coord.y][p2_Coord.x + 1] = P2_SELECTED;
				if (map_Status[p2_Coord.y][p2_Coord.x] != P2_OWNED) {
					p2_select[p2_Coord.y][p2_Coord.x]->setVisible(true);
				}
				temp_Coord = p2_Coord;
				temp_Coord.x++;
				p2_Movement.push_back(temp_Coord);
				p2_cursor->runAction(moveRight);
				Classic_Ingame::MissionFailedCheck(2);
				break;
			}
		}
	case EventKeyboard::KeyCode::KEY_SHIFT:
		if (p2_Movement.size() >= 3 && p2_Movement.size() <= 6) {
			Classic_Ingame::MissionClearCheck(2);
			break;
		}
		else if (p2_Movement.size() != 0) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_INCORRECT, false);
			p2_cursor->setPosition(GetTile(p2_Movement.front().x, p2_Movement.front().y));
			TileDataClear(2);
			p2_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p2_Movement.push_back(p2_Coord);
			SetPlayerSpeechBubble(2, p2_AvatarSelect, FAILED);
			break;
		}
		else { break; }
	}
}
void Classic_Ingame::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		p1_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		p1_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		p1_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		p1_isMoved = FALSE;
		break;

	case EventKeyboard::KeyCode::KEY_W:
		p2_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		p2_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		p2_isMoved = FALSE;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		p2_isMoved = FALSE;
		break;
	}
}

Vec2 Classic_Ingame::GetTile(int inputX, int inputY) {
	float background_x = visibleSize.width / 2 - board_Background->getContentSize().width / 2;
	float background_y = visibleSize.height / 2 - board_Background->getContentSize().height / 2;

	Vec2 Tile;

	Tile.x = background_x + 50 * inputX;
	Tile.y = background_y + 50 * (11 - inputY);

	return Tile;
}
float Classic_Ingame::GetTileElement(int inputX, int inputY, int return_type) {
	float background_x = visibleSize.width / 2 - board_Background->getContentSize().width / 2;
	float background_y = visibleSize.height / 2 - board_Background->getContentSize().height / 2;

	Vec2 Tile;

	Tile.x = background_x + 50 * (inputX - 1);
	Tile.y = background_y + 50 * (12 - inputY);

	if (return_type == 1) { return Tile.x; }
	else if (return_type == 2) { return Tile.y; }
	else { return 0; }
}

void Classic_Ingame::GetCursorCoord() {
	float background_x = visibleSize.width / 2 - board_Background->getContentSize().width / 2;
	float background_y = visibleSize.height / 2 - board_Background->getContentSize().height / 2;

	p1_Coord.x = (p1_cursor->getPositionX() - background_x) / 50;
	p2_Coord.x = (p2_cursor->getPositionX() - background_x) / 50;
	p1_Coord.y = 11 - (p1_cursor->getPositionY() - background_y) / 50;
	p2_Coord.y = 11 - (p2_cursor->getPositionY() - background_y) / 50;
}

void Classic_Ingame::SetPlayerSpeechBubble(int player, int character_type, int type) {
	std::stringstream dir;
	dir << "UI/bubble/";

	switch (type) {
	case NOTHING:
		dir << "speech_bubble.png";
		break;
	case SUCCESS:
		dir << character_type << "/pair_success.png";
		break;
	case PERFECT:
		dir << character_type << "/pair_perfect.png";
		break;
	case FAILED:
		dir << character_type << "/pair_failed.png";
		break;
	case VICTORY:
		dir << character_type << "/victory.png";
		break;
	case DEFEAT:
		dir << character_type << "/defeat.png";
		break;
	}

	switch (player) {
	case 1:
		if (FileUtils::getInstance()->isFileExist(dir.str())) {
			if (type != NOTHING) {
				p1_SpeechRemainTime = 3;
			}
			p1_SpeechBubble->setTexture(dir.str());
		}
		break;
	case 2:
		if (FileUtils::getInstance()->isFileExist(dir.str())) {
			if (type != NOTHING) {
				p2_SpeechRemainTime = 3;
			}
			p2_SpeechBubble->setTexture(dir.str());
		}
		break;
	}
}

void Classic_Ingame::TileDataClear(int player) {
	switch (player) {
	case 1:
		for (Coord coord : p1_Movement) {
			map_Status[coord.y][coord.x] = NORMAL;
			p1_select[coord.y][coord.x]->setVisible(false);
		}
		break;
	case 2:
		for (Coord coord : p2_Movement) {
			map_Status[coord.y][coord.x] = NORMAL;
			p2_select[coord.y][coord.x]->setVisible(false);
		}
		break;
	}
}

void Classic_Ingame::MissionFailedCheck(int player) {
	switch (player) {
	case 1:
		if (p1_Movement.size() > mission_Range) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_BREAK);
			TileDataClear(1);
			p1_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p1_Movement.push_back(temp_Coord);
			map_Status[temp_Coord.y][temp_Coord.x] = P1_SELECTED;
		}
		break;
	case 2:
		if (p2_Movement.size() > mission_Range) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_BREAK);
			TileDataClear(2);
			p2_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p2_Movement.push_back(temp_Coord);
			map_Status[temp_Coord.y][temp_Coord.x] = P1_SELECTED;
		}
		break;
	default:
		break;
	}
}

void Classic_Ingame::MissionClearCheck(int player) {
	switch (player) {
	case 1:
		if (tile[p1_Movement.front().y][p1_Movement.front().x]->getTexture() == tile[p1_Movement.back().y][p1_Movement.back().x]->getTexture()) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_CORRECT, false);
			for (Coord coord : p1_Movement) {
				map_Status[coord.y][coord.x] = P1_OWNED;
				tile[coord.y][coord.x]->setColor(Color3B::RED);
				tile[coord.y][coord.x]->setOpacity(255);
				p1_select[coord.y][coord.x]->setVisible(false);
			}
			p1_Score += 1000 - (6 - p1_Movement.size()) * 200;
			p1_MissionProgress++;
			if (p1_Movement.size() == mission_Range) {
				SetPlayerSpeechBubble(1, p1_AvatarSelect, PERFECT);
			}
			else {
				SetPlayerSpeechBubble(1, p1_AvatarSelect, SUCCESS);
			}
			p1_Movement.clear();
			break;
		}
		else {
			SetPlayerSpeechBubble(1, p1_AvatarSelect, FAILED);
			TileDataClear(1);
			p1_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p1_Movement.push_back(p1_Coord);
			map_Status[p1_Coord.y][p1_Coord.x] = P1_SELECTED;
			SimpleAudioEngine::sharedEngine()->playEffect(FX_INCORRECT, false);
			break;
		}
	case 2:
		if (tile[p2_Movement.front().y][p2_Movement.front().x]->getTexture() == tile[p2_Movement.back().y][p2_Movement.back().x]->getTexture()) {
			SimpleAudioEngine::sharedEngine()->playEffect(FX_CORRECT, false);
			for (Coord coord : p2_Movement) {
				map_Status[coord.y][coord.x] = P2_OWNED;
				tile[coord.y][coord.x]->setColor(Color3B::BLUE);
				tile[coord.y][coord.x]->setOpacity(255);
				p2_select[coord.y][coord.x]->setVisible(false);
			}
			p2_Score += 1000 - (6 - p2_Movement.size()) * 200;
			p2_MissionProgress++;
			if (p2_Movement.size() == mission_Range) {
				SetPlayerSpeechBubble(2, p2_AvatarSelect, PERFECT);
			}
			else {
				SetPlayerSpeechBubble(2, p2_AvatarSelect, SUCCESS);
			}
			p2_Movement.clear();
			break;
		}
		else {
			SetPlayerSpeechBubble(2, p2_AvatarSelect, FAILED);
			TileDataClear(2);
			p2_Movement.clear();
			Classic_Ingame::GetCursorCoord();
			p2_Movement.push_back(p2_Coord);
			map_Status[p2_Coord.y][p2_Coord.x] = P2_SELECTED;
			SimpleAudioEngine::sharedEngine()->playEffect(FX_INCORRECT, false);
			break;
		}
	}
}

void Classic_Ingame::SpriteAnimationInitialize(int player, int select) {
	std::stringstream dir;
	dir << "Avatar/" << select << "/";

	if (FileUtils::getInstance()->isDirectoryExist(dir.str())) {
		Vector<SpriteFrame *> frame;
		bool loop = true;
		int fileCounter = 0;

		do {
			std::stringstream temp;
			temp << dir.str() << "frame_" << fileCounter << ".png";

			if (FileUtils::getInstance()->isFileExist(temp.str())) {
				if (fileCounter == 0) {
					switch (player) {
					case 1:
						p1_Avatar->setTexture(temp.str());
						break;
					case 2:
						p2_Avatar->setTexture(temp.str());
						break;
					}
				}

				switch (player) {
				case 1:
					frame.pushBack(SpriteFrame::create(temp.str(), Rect(0, 0, p1_Avatar->getContentSize().width, p1_Avatar->getContentSize().height)));
					break;
				case 2:
					frame.pushBack(SpriteFrame::create(temp.str(), Rect(0, 0, p1_Avatar->getContentSize().width, p1_Avatar->getContentSize().height)));
					break;
				}

				fileCounter++;
			}
			else {
				loop = false;
			}
		} while (loop);

		if (fileCounter > 0) {
			Animation * animation = Animation::createWithSpriteFrames(frame, 0.1f);
			Animate* animate = Animate::create(animation);

			switch (player) {
			case 1:
				p1_Avatar->runAction(RepeatForever::create(animate));
				break;
			case 2:
				p2_Avatar->runAction(RepeatForever::create(animate));
				break;
			}
		}
	}
}

void Classic_Ingame::PlayCountDown(float delta) {
	FadeIn *fadein = FadeIn::create(0.2f);
	DelayTime *delay1 = DelayTime::create(0.6f);
	FadeOut *fadeout = FadeOut::create(0.2f);
	Sequence *count = Sequence::create(fadein, delay1, fadeout, NULL);

	count_down->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	count_down->setString(StringUtils::format("%d", countDown));
	count_down->runAction(count);
	countDown--;

	if (countDown < 0) {
		this->unschedule(schedule_selector(Classic_Ingame::PlayCountDown));
		this->removeChild(count_down);

		auto keylistener = EventListenerKeyboard::create();
		keylistener->onKeyPressed = CC_CALLBACK_2(Classic_Ingame::onKeyPressed, this);
		keylistener->onKeyReleased = CC_CALLBACK_2(Classic_Ingame::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

		this->schedule(schedule_selector(Classic_Ingame::update));
		this->schedule(schedule_selector(Classic_Ingame::timecheck), 1);
	}
}

void Classic_Ingame::Gameover(bool winner) { //1p win -> winner = true / 2p win -> winner = false
	p1_DisplayScore = p1_Score;
	p2_DisplayScore = p2_Score;

	score1->setString(StringUtils::format("%d", p1_DisplayScore));
	score2->setString(StringUtils::format("%d", p2_DisplayScore));

	if (winner == true) {
		UserDefault::getInstance()->setStringForKey("TempWinner", "Player 1");
		UserDefault::getInstance()->setIntegerForKey("TempScoreGap", p1_Score - p2_Score);
		UserDefault::getInstance()->setIntegerForKey("TempRemainTime", remainTime);
		SetPlayerSpeechBubble(1, p1_AvatarSelect, VICTORY);
		SetPlayerSpeechBubble(2, p2_AvatarSelect, DEFEAT);
	}
	else {
		UserDefault::getInstance()->setStringForKey("TempWinner", "Player 2");
		UserDefault::getInstance()->setIntegerForKey("TempScoreGap", p2_Score - p1_Score);
		UserDefault::getInstance()->setIntegerForKey("TempRemainTime", remainTime);
		SetPlayerSpeechBubble(1, p1_AvatarSelect, DEFEAT);
		SetPlayerSpeechBubble(2, p2_AvatarSelect, VICTORY);
	}

	MoveTo *boxDrop = MoveTo::create(1.0f, Point(640, 360));
	CallFunc *dropSound = CallFunc::create(CC_CALLBACK_0(Classic_Ingame::GameoverFX, this));
	Sequence *gameoverDrop = Sequence::create(boxDrop, dropSound, NULL);
	gameoverBox->setVisible(true);
	gameoverBox->runAction(gameoverDrop);

	this->unscheduleAllSelectors();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	this->scheduleOnce(schedule_selector(Classic_Ingame::GameoverChangeScene), 5.0f);
}

void Classic_Ingame::GameoverFX() {
	SimpleAudioEngine::sharedEngine()->playEffect(FX_GAMEOVER);
}

void Classic_Ingame::GameoverChangeScene(float delta) {
	Director::getInstance()->replaceScene(TransitionFadeUp::create(1.0f, tempScene::createScene()));
}

void Classic_Ingame::update(float delta) {
	Classic_Ingame::GetCursorCoord();

	if (p1_DisplayScore != p1_Score) {
		p1_DisplayScore += 10;
		score1->setString(StringUtils::format("%d", p1_DisplayScore));
	}

	if (p2_DisplayScore != p2_Score) {
		p2_DisplayScore += 10;
		score2->setString(StringUtils::format("%d", p2_DisplayScore));
	}

	if (remainTime <= 0) {
		if (p1_Score > p2_Score) {
			Gameover(true);
		}
		else if (p2_Score > p1_Score) {
			Gameover(false);
		}
		else {
			this->unscheduleAllSelectors();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->end();
		}
	}
	if (p1_MissionProgress == mission_Pair || p2_MissionProgress == mission_Pair) {
		if (p1_MissionProgress == p2_MissionProgress) {
			this->unscheduleAllSelectors();
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			Director::getInstance()->end();
		}
		else if (p1_MissionProgress == mission_Pair) {
			Gameover(true);
		}
		else if (p2_MissionProgress == mission_Pair) {
			Gameover(false);
		}
	}
}
void Classic_Ingame::timecheck(float delta) {
	remainTime--;
	if (p1_SpeechRemainTime > 0) {
		p1_SpeechRemainTime--;
		if (p1_SpeechRemainTime <= 0) {
			Classic_Ingame::SetPlayerSpeechBubble(1, p1_AvatarSelect, NOTHING);
		}
	}

	if (p2_SpeechRemainTime > 0) {
		p2_SpeechRemainTime--;
		if (p2_SpeechRemainTime <= 0) {
			Classic_Ingame::SetPlayerSpeechBubble(2, p2_AvatarSelect, NOTHING);
		}
	}
	timeUI->setTextureRect(Rect(timeUI_OriginMinX, timeUI_OriginMinY, timeUI_OriginMaxX, timeUI_OriginMaxY * remainTime / 60));
	timeUI->setPosition(Vec2(background->getPositionX(), 720 - 690));
}