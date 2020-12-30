#include "Mission.h"

Mission::Mission(int l, int b)
: m_iLines(l)
, m_iBoxes(b)
, m_iMinimumBoxes(0)
, m_iIconID(0)
, m_bFindIcon(false)
, m_bMinimumRequired(false)
, m_bInLine(false)
, m_sMissionText("")
, m_eMissionType(MISSION_LINEANDBOXES)
{
	
}

Mission::~Mission()
{

}

void Mission::setLines(int l)
{
	this->m_iLines = l;
}

int Mission::getLines()
{
	return this->m_iLines;
}

void Mission::setBoxes(int b)
{
	this->m_iBoxes = b;
}

int Mission::getBoxes()
{
	return this->m_iBoxes;
}

void Mission::setMinimumBoxes(int b)
{
	this->m_iMinimumBoxes = b;
}

int Mission::getMinimumBoxes()
{
	return this->m_iMinimumBoxes;
}

void Mission::generateNewMission()
{
	int lines = cocos2d::random(1, 3);
	int boxes = cocos2d::random(1, 6);

	this->m_iLines = lines;
	this->m_iBoxes = boxes;
	this->m_eMissionType = MISSION_LINEANDBOXES;
}

bool Mission::checkMissionBoxes(int size)
{
	if (size <= m_iBoxes) // >=
	{
		return true;
	}
	return false;
}

int Mission::getScorePoints(int size)
{
	// Default Score
	return (200 * (size)); // size - 1
}

void Mission::setMissionText(std::string s)
{
	this->m_sMissionText = s;
}

std::string Mission::getMissionText()
{
	return this->m_sMissionText;
}

void Mission::setMissionType(MISSION_TYPE e)
{
	this->m_eMissionType = e;
}

Mission::MISSION_TYPE Mission::getMissionType()
{
	return this->m_eMissionType;
}

void Mission::setIconID(int i)
{
	this->m_iIconID = i;
}

int Mission::getIconID()
{
	return this->m_iIconID;
}

void Mission::setFindIcon(bool b)
{
	this->m_bFindIcon = b;
}

bool Mission::getFindIcon()
{
	return this->m_bFindIcon;
}

void Mission::setMinimumRequired(bool b)
{
	this->m_bMinimumRequired = b;
}

bool Mission::getMinimumRequired()
{
	return this->m_bMinimumRequired;
}

void Mission::setInLine(bool b)
{
	this->m_bInLine = b;
}

bool Mission::getInLine()
{
	return this->m_bInLine;
}