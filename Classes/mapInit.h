#ifndef __MAP_INIT_
#define __MAP_INIT_

#include "cocos2d.h"

USING_NS_CC;

enum MAPTYPE { SQUARE, ACORN, CIRCLE, HEART, HARD };

class mapInit
{
private:
	static mapInit* p_instance;
	mapInit(const mapInit&) = delete;
	mapInit& operator = (const mapInit&) = delete;
	explicit mapInit() = default;
	~mapInit() = default;

	int block[9][16];
	int data_squareMap[12][12];

public:
	static mapInit* GetInstance()
	{
		if (p_instance == nullptr)
		{
			p_instance = new mapInit();
		}
		return p_instance;
	};
	static void Release() {
		if (p_instance) {
			delete p_instance;
			p_instance = nullptr;
		}
	};

	void Initialize(int mapType);
	int GetTile(int x, int y);
	void ClearData();

};

#endif