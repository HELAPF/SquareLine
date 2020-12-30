#include "Grid.h"

#include "Gameboard.h"

Grid::Grid() 
: m_cGridSprite(nullptr)
, m_bOccupied(false)
, m_iIconID(0)
, m_bActive(true)
, m_bHasClone(false)
, m_bChanging(false)
{

}

Grid::Grid(const std::string &filepath)
: m_bHasClone(false)
, m_iIconID(0)
, m_bOccupied(false)
, m_bChanging(false)
{
	this->m_cGridSprite = Sprite::create(filepath);
	this->m_cGridSprite->setName("Grid Sprite");
	this->addChild(this->m_cGridSprite, -1);
}

Grid::~Grid()
{

}

void Grid::setSprite(cocos2d::Sprite * sprite)
{
	this->m_cGridSprite = sprite;
}

cocos2d::Sprite * Grid::getSprite()
{
	return this->m_cGridSprite;
}

int Grid::getActive()
{
	return this->m_bActive; 

}

void Grid::setActive(bool a,bool b)
{ 
	if (!a && b)
	{
		this->m_cGridSprite->setColor(Color3B::BLACK);
	}
	this->m_bActive = a; 
}

void Grid::Reset()
{
	if (m_cGridSprite)
	{
		this->m_cGridSprite->removeFromParent();
		this->m_cGridSprite = nullptr;
	}
	this->removeAllChildren();
	this->m_bOccupied = false;
	this->m_iIconID = 0;
	this->m_bActive = true;
	this->m_bHasClone = false;
	this->m_bChanging = false;
}

void Grid::FakeReset()
{
	this->Reset();
	this->m_cGridSprite = Sprite::create("Icons/iconplaceholder.png");
	this->addChild(this->m_cGridSprite, -1);
}

void Grid::Destroy()
{
	this->FakeReset();
	this->m_bHasClone = true;
}

void Grid::SpawnObstacle()
{
	this->m_bHasClone = true;
	auto fadein = FadeIn::create(1.f);
	auto sprite = Sprite::create("Cursors/Obstacle.png");
	sprite->setName("Obstacle");
	sprite->setOpacity(0);
	sprite->setScale(0.95f);
	sprite->runAction(fadein);
	this->addChild(sprite, 1);
}

void Grid::ChangeIcon()
{
	do
	{
		int newIconID = cocos2d::random(1, 8);
		if (newIconID == this->m_iIconID)
		{
			continue;
		}
		this->removeChildByName("Grid Sprite");
		this->m_bChanging = true;
		this->m_iIconID = newIconID;
		this->setOccupied(true);
		std::stringstream ss;
		ss << "Icons/icon" << newIconID << ".png";
		auto newSprite = Sprite::create(ss.str());
		newSprite->setName("Grid Sprite");
		this->setSprite(newSprite);
		this->addChild(newSprite, -1);
		break;
	} 
	while (true);
}