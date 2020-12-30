#include "Timer.h"
#include "AIComponent.h"

StageModeTimer::StageModeTimer(float time)
: m_bStart(false)
, m_fTimeLimit(time)
, m_fCurrentTime(time)
{
	auto timerSprite = Sprite::create("UI/timer.png");
	timerSprite->setName("Timer Sprite");
	timerSprite->setScaleY(0.5f);
	timerSprite->setScaleX(1.f);
	timerSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * (MIDDLE_WIDTH_OFFSET - 0.01f), (Director::getInstance()->getVisibleSize().height * TIMER_HEIGHT_OFFSET)));
	this->addChild(timerSprite, 1);

	auto timer = ProgressTimer::create(Sprite::create("UI/time.png"));
	timer->setName("Timer");
	timer->setScaleX(0.99f);
	timer->setPosition(Vec2(timerSprite->getContentSize().width * (MIDDLE_WIDTH_OFFSET - 0.006f), timerSprite->getContentSize().height * 0.55f));
	timer->setType(ProgressTimer::Type::BAR);
	timer->setPercentage(100.f);
	timer->setMidpoint(Vec2(0, 1));
	timer->setBarChangeRate(Vec2(1, 0));
	timerSprite->addChild(timer, 1);
}

StageModeTimer::~StageModeTimer()
{

}

void StageModeTimer::update(float time, AIComponent * aic)
{
	if (this->m_bStart)
	{
		auto timer = dynamic_cast<ProgressTimer*>(this->getChildByName("Timer Sprite")->getChildByName("Timer"));
		this->m_fCurrentTime -= time;
		aic->setTimeElapsed(aic->getTimeElapsed() + time);
		timer->setPercentage(100 * m_fCurrentTime / m_fTimeLimit);
	}
}

void StageModeTimer::setTimeLimit(float time)
{
	this->m_fTimeLimit = time;
}

float StageModeTimer::getTimeLimit()
{
	return this->m_fTimeLimit;
}

void StageModeTimer::setCurrentTime(float time)
{
	this->m_fCurrentTime = time;
}

float StageModeTimer::getCurrentTime()
{
	return this->m_fCurrentTime;
}

void StageModeTimer::start()
{
	this->m_bStart = true;
}

void StageModeTimer::setStart(bool start)
{
	this->m_bStart = start;
}

bool StageModeTimer::getStart()
{
	return this->m_bStart;
}