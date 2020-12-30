#ifndef __MISSION_H__
#define __MISSION_H__

#include "cocos2d.h"
#include "Grid.h"

USING_NS_CC;

class Mission
{
public:
	enum MISSION_TYPE
	{
		MISSION_LINEANDBOXES,
		MISSION_CUSTOM,
	};

public:
	Mission(int l = 1, int b = 1);
	~Mission();

	void generateNewMission();
	bool checkMissionBoxes(int);
	int getScorePoints(int);
public:
	void setLines(int);
	int getLines();

	void setBoxes(int);
	int getBoxes();

	void setMinimumBoxes(int);
	int getMinimumBoxes();

	void setMissionText(std::string);
	std::string getMissionText();

	void setMissionType(MISSION_TYPE);
	MISSION_TYPE getMissionType();

	void setIconID(int);
	int getIconID();

	void setFindIcon(bool);
	bool getFindIcon();

	void setMinimumRequired(bool);
	bool getMinimumRequired();

	void setInLine(bool);
	bool getInLine();

private:
	int m_iLines;
	int m_iBoxes;
	int m_iMinimumBoxes;
	int m_iIconID;
	bool m_bFindIcon;
	bool m_bMinimumRequired;
	bool m_bInLine;
	std::string m_sMissionText;
	MISSION_TYPE m_eMissionType;
};

#endif