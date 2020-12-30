#ifndef __AI_COMPONENT_H__
#define __AI_COMPONENT_H__

#include "StageModeNPC.h"

class AIComponent
{
public:
	enum AI_PATTERN
	{
		PATTERN_SPAWN_OBSTACLE_RANDOM,
		PATTERN_SPAWN_OBSTACLE_LINE,
		PATTERN_NATURE,
		PATTERN_CHAIN,
		PATTERN_FIND_SPECIFICICON,
		PATTERN_RESETMAP,
		PATTERN_DESTROYICON,
		PATTERN_CHANGINGTILES,
		PATTERN_NONE,
	};
public:
	AIComponent(StageModeNPC::NPC);
	~AIComponent();

	void setPattern(AI_PATTERN);
	AI_PATTERN getPattern();

	void setObstacleSpawnTimer(float);
	float getObstacleSpawnTimer();

	void setObstacleSpawnTime(float);
	float getObstacleSpawnTime();

	void setResetTime(float);
	float getResetTime();

	void setTimeElapsed(float);
	float getTimeElapsed();

	void setDestroyTime(float);
	float getDestroyTime();

	void setObstaclesToSpawn(int);
	int getObstaclesToSpawn();

	void setIconToFind(int);
	int getIconToFind();

	void setSpawnAround(bool);
	bool getSpawnAround();;

	void setSpawnDiagonal(bool);
	bool getSpawnDiagonal();

	void setResetMap(bool);
	bool getResetMap();

	void setFindIcon(bool);
	void generateNewIconID();
	bool getFindIcon();

	void addToDestroyedList(int);
	std::vector<int> getDestroyedList();
	bool findDestroyed(int);

private:
	AI_PATTERN m_ePattern;
	float m_fObstacleSpawnTimer;
	float m_fObstacleSpawnTime;
	float m_fResetTime;
	float m_fTimeElapsed;
	float m_fDestroyTime;
	int m_iObstaclesToSpawn;
	int m_iIconIDToFind;
	bool m_bSpawnAround;
	bool m_bSpawnDiagonal;
	bool m_bResetMap;
	bool m_bFindIcon;
	std::vector<int> m_iDestroyedList;
};

#endif