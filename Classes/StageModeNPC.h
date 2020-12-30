#ifndef __STAGE_MODE_NPC__
#define __STAGE_MODE_NPC__

#include "cocos2d.h"
#include <string>
#include "GameBoard.h"
#include "StageModePlayer.h"

class Grid;
class AIComponent;

USING_NS_CC;

class StageModeNPC : public cocos2d::Node
{
public:
	enum NPC
	{
		NPC_BISON = 1,
		NPC_TOSCAYA,
		NPC_CEILTA,
		NPC_HAJIME,
		NPC_POKA,
		NPC_TSUBASA,
		NPC_CHINGCHING,
		NPC_ASELLIA,
		NPC_YORSHTA,
		NPC_ABYSSASELLIA,
		NPC_TOTAL,
	};

public:
	StageModeNPC(int);
	~StageModeNPC();

	void runPattern(GameBoard*,StageModePlayer*,float);
	Sprite * spawnObstacle();
	void updateEvent();

public:
	void setID(int);
	int getID();

	void setName(std::string);
	std::string getName();

	void setAIComponent(AIComponent*);
	AIComponent * getAIComponent();

	void setText(std::string);
	std::string getText();

	void setScoreLines(bool);
	bool getScoreLines();

	std::vector<int> getScoreList();

private:
	int m_iID;
	std::string m_sName;
	std::string m_sNPCText;
	AIComponent * m_cAIComponent;
	std::vector<int> m_iScoreList;
	bool m_bScoreLines;
};

#endif