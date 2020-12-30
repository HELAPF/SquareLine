#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"

#include "Defines.h"

USING_NS_CC;

#include "BoardPiece.h"

class Grid : public cocos2d::Node
{
public:
	Grid();
	Grid(const std::string &filename);
	~Grid();

public: 
	// Sprite Set and Getters
	void setSprite(cocos2d::Sprite*);
	cocos2d::Sprite * getSprite();

	// General Functions
	bool getOccupied() { return this->m_bOccupied; }
	void setOccupied(bool o) { this->m_bOccupied = o; }

	int getIconID() { return this->m_iIconID; }
	void setIconID(int i) { this->m_iIconID = i; }

	int getActive();
	void setActive(bool,bool a = true);

	bool getCloned() { return m_bHasClone; }
	void setCloned(bool c) { this->m_bHasClone = c; }

	void setChanging(bool b) { this->m_bChanging = b; }
	bool getChanging() { return this->m_bChanging; }

	void Reset();
	void FakeReset();
	void Destroy();
	void SpawnObstacle();
	void ChangeIcon();

private:
	cocos2d::Sprite * m_cGridSprite;
	bool m_bOccupied;
	bool m_bActive;
	bool m_bHasClone;
	bool m_bChanging;
	int m_iIconID;
};

#endif