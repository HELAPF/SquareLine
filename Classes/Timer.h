#ifndef __TIMER_H__
#define __TIMER_H__

#include "cocos2d.h"
#include "Defines.h"

USING_NS_CC;

class AIComponent;

class StageModeTimer : public cocos2d::Node
{
public:
	StageModeTimer(float);
	~StageModeTimer();

	virtual void update(float,AIComponent*);

public:
	void setTimeLimit(float); // Seconds
	float getTimeLimit();

	void setCurrentTime(float);
	float getCurrentTime();

	void start();
	void setStart(bool);
	bool getStart();
private:
	bool m_bStart;
	float m_fTimeLimit;
	float m_fCurrentTime;
};

#endif