#ifndef __Classic_Ingame_
#define __Classic_Ingame_

///////////////
// file path
#define FILE1 "picture/pic1.png"
#define FILE2 "picture/pic2.png"
#define FILE3 "picture/pic3.png"
#define FILE4 "picture/pic4.png"
#define FILE5 "picture/pic5.png"
#define FILE6 "picture/pic6.png"
#define FILE7 "picture/pic7.png"
#define FILE8 "picture/pic8.png"
#define FILE9 "picture/pic9.png"

#define FX_MOVE "FX/Beep_Ping.wav"
#define FX_CORRECT "FX/Door_Bell.wav"
#define FX_INCORRECT "FX/Buzz.wav"
#define FX_GAMEOVER "UI/result/Sound.wav"
#define FX_BREAK "FX/break_event.wav"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "mapInit.h"
#include "tempScene.h"
#include <vector>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

class Classic_Ingame : public cocos2d::Layer {
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Background Layout
	Sprite *background;
	Sprite *board_Background;

	///////////////
	// Player

	//Player Cursor
	Sprite *p1_cursor;
	Sprite *p2_cursor;

	struct Coord {
		int x, y;
	};
	Coord p1_Coord;
	Coord p2_Coord;
	Coord temp_Coord;

	//Cursor movement data
	vector<Coord> p1_Movement;
	vector<Coord> p2_Movement;

	//UI
	Sprite *p1_ScoreUI;
	Sprite *p1_SpeechBubble;
	Sprite *p1_Avatar;
	int p1_AvatarSelect;
	Sprite *p1_Name;

	Sprite *p2_ScoreUI;
	Sprite *p2_SpeechBubble;
	Sprite *p2_Avatar;
	int p2_AvatarSelect;
	Sprite *p2_Name;

	Sprite *missionUI;
	Sprite *timeBackground;
	Sprite *timeUI;
	float timeUI_OriginMinX;
	float timeUI_OriginMaxX;
	float timeUI_OriginMinY;
	float timeUI_OriginMaxY;

	Sprite *gameoverBox;

	//Input limit
	bool p1_isMoved;
	bool p2_isMoved;

	///////////////
	// Maptile

	//Tile image
	Sprite *tile[12][12];
	Sprite *p1_select[12][12];
	Sprite *p2_select[12][12];

	//Tile coordinate
	Vec2 GetTile(int inputX, int inputY);
	float GetTileElement(int inputX, int inputY, int return_type);
	//Map status
	enum TILE_STATUS { NORMAL = 0, P1_SELECTED, P2_SELECTED, P1_OWNED, P2_OWNED, BLOCKED };
	int map_Status[12][12];

	///////////////
	// Gameplay

	int countDown;

	int mission_Range;
	int mission_Pair;

	int p1_MissionProgress;
	int p2_MissionProgress;

	int p1_DisplayScore;
	int p2_DisplayScore;

	int p1_Score;
	int p2_Score;

	int remainTime;
	int p1_SpeechRemainTime;
	int p2_SpeechRemainTime;

	Label *count_down;
	LabelTTF *mission;
	Label *score1;
	Label *score2;

public:
	static cocos2d::Scene *createScene();
	virtual bool init();

	//Game Initialize
	void SpriteAnimationInitialize(int player, int select);
	void PlayCountDown(float delta);
	//Game Finalize
	void GameoverFX();
	void Gameover(bool winner);
	void GameoverChangeScene(float delta);

	///////////////
	// Update

	void update(float delta);
	void timecheck(float delta);
	//update function
	void GetCursorCoord();
	enum SPEECH_TYPE { NOTHING, SUCCESS, PERFECT, FAILED, VICTORY, DEFEAT };
	void SetPlayerSpeechBubble(int player, int charachter_type, int type);
	//Tile check
	void TileDataClear(int player);
	void MissionFailedCheck(int player);
	void MissionClearCheck(int player);

	///////////////
	// Keyboard Listener

	void onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(Classic_Ingame);
};

#endif