#include "AIComponent.h"

AIComponent::AIComponent(StageModeNPC::NPC id)
: m_ePattern(PATTERN_NONE)
, m_fObstacleSpawnTimer(0.f)
, m_fObstacleSpawnTime(0.f)
, m_fResetTime(0.f)
, m_fTimeElapsed(0.f)
, m_fDestroyTime(0.f)
, m_iObstaclesToSpawn(0)
, m_iIconIDToFind(-1)
, m_bSpawnAround(false)
, m_bSpawnDiagonal(false)
, m_bResetMap(false)
, m_bFindIcon(false)
{
	switch (StageModeNPC::NPC(id))
	{
	case StageModeNPC::NPC::NPC_BISON:
		this->m_ePattern = PATTERN_SPAWN_OBSTACLE_RANDOM;
		this->m_fObstacleSpawnTime = 3.f;
		this->m_iObstaclesToSpawn = 1;
		break;
	case StageModeNPC::NPC::NPC_TOSCAYA:
		this->m_ePattern = PATTERN_SPAWN_OBSTACLE_LINE;
		this->m_iObstaclesToSpawn = 2;
		this->m_bSpawnAround = true;
		break;
	case StageModeNPC::NPC::NPC_CEILTA:
		this->m_ePattern = PATTERN_SPAWN_OBSTACLE_LINE;
		this->m_iObstaclesToSpawn = 4;
		this->m_bSpawnDiagonal = true;
		break;
	case StageModeNPC::NPC::NPC_HAJIME:
		this->m_ePattern = PATTERN_FIND_SPECIFICICON;
		this->generateNewIconID();
		this->m_bFindIcon = true;
		break;
	case StageModeNPC::NPC::NPC_POKA:
		this->m_ePattern = PATTERN_RESETMAP;
		this->m_fResetTime = 15.f;
		break;
	case StageModeNPC::NPC::NPC_TSUBASA:
		this->m_ePattern = PATTERN_DESTROYICON;
		this->m_fDestroyTime = 15.f;
		break;
	case StageModeNPC::NPC::NPC_CHINGCHING:
		this->m_ePattern = PATTERN_CHANGINGTILES;
		break;
	case StageModeNPC::NPC::NPC_ASELLIA:
		this->m_ePattern = PATTERN_NATURE;
		break;
	case StageModeNPC::NPC::NPC_YORSHTA:
		this->m_ePattern = PATTERN_CHAIN;
		this->m_fResetTime = 45.f;
		break;
	case StageModeNPC::NPC::NPC_ABYSSASELLIA:
		this->m_ePattern = PATTERN_FIND_SPECIFICICON;
		this->generateNewIconID();
		this->m_bFindIcon = true;
		break;
	}
}

AIComponent::~AIComponent()
{

}

void AIComponent::setPattern(AI_PATTERN pattern)
{
	this->m_ePattern = pattern;
}

AIComponent::AI_PATTERN AIComponent::getPattern()
{
	return this->m_ePattern;
}

void AIComponent::setObstacleSpawnTimer(float time)
{
	this->m_fObstacleSpawnTimer = time;
}

float AIComponent::getObstacleSpawnTimer()
{
	return this->m_fObstacleSpawnTimer;
}

void AIComponent::setObstacleSpawnTime(float time)
{
	this->m_fObstacleSpawnTime = time;
}

float AIComponent::getObstacleSpawnTime()
{
	return this->m_fObstacleSpawnTime;
}

void AIComponent::setResetTime(float f)
{
	this->m_fResetTime = f;
}

float AIComponent::getResetTime()
{
	return this->m_fResetTime;
}

void AIComponent::setTimeElapsed(float time)
{
	this->m_fTimeElapsed = time;
}

float AIComponent::getTimeElapsed()
{
	return this->m_fTimeElapsed;
}

void AIComponent::setDestroyTime(float f)
{
	this->m_fDestroyTime = f;
}

float AIComponent::getDestroyTime()
{
	return this->m_fDestroyTime;
}

void AIComponent::setObstaclesToSpawn(int i)
{
	this->m_iObstaclesToSpawn = i;
}

int AIComponent::getObstaclesToSpawn()
{
	return this->m_iObstaclesToSpawn;
}

void AIComponent::setIconToFind(int i)
{
	this->m_iIconIDToFind = i;
}

int AIComponent::getIconToFind()
{
	return this->m_iIconIDToFind;
}

void AIComponent::setSpawnAround(bool b)
{
	this->m_bSpawnAround = b;
}

bool AIComponent::getSpawnAround()
{
	return this->m_bSpawnAround;
}

void AIComponent::setSpawnDiagonal(bool b)
{
	this->m_bSpawnDiagonal = b;
}

bool AIComponent::getSpawnDiagonal()
{
	return this->m_bSpawnDiagonal;
}

void AIComponent::setResetMap(bool b)
{
	this->m_bResetMap = b;
}

bool AIComponent::getResetMap()
{
	return this->m_bResetMap;
}

void AIComponent::setFindIcon(bool b)
{
	this->m_bFindIcon = b;
}

void AIComponent::generateNewIconID()
{
	do
	{
		int newIconID = cocos2d::random(1, 8);
		if (newIconID != this->m_iIconIDToFind)
		{
			this->m_iIconIDToFind = newIconID;
			break;
		}
	} while (true);
}

bool AIComponent::getFindIcon()
{
	return this->m_bFindIcon;
}

void AIComponent::addToDestroyedList(int i)
{
	this->m_iDestroyedList.push_back(i);
}

std::vector<int> AIComponent::getDestroyedList()
{
	return this->m_iDestroyedList;
}

bool AIComponent::findDestroyed(int d)
{
	for(int i : m_iDestroyedList)
	{
		if (i == d)
		{
			return true;
		}
	}
	return false;
}



