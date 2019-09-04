/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	bomb.h
Description	:	Bomb object class
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/
#pragma once
#include "sprite.h"
#include "robot.h"

class Bomb : public GameObject
{
public:
	Bomb(Level* _level);
	virtual ~Bomb();

	virtual void Initialise();
	virtual void Update(double _dTime);
	virtual void Draw();

	void SetBombRadius(float _radius);
	void CountCasualties();

	int GetBombScore() const;
	float GetBombRadius() const;
	int GetGoodDestroyed() const;
	int GetBadDestroyed() const;
	int GetCasualties() const;

private:
	Sprite * sprite;
	Texter * textA;
	std::vector<Robot*>* vecRobots;
	float bombRadius;	//0.0 -> 1.0
	int bombScore;		//How effective the bombing will be
	int casualties;		//Total number of bots destroyed
	int goodDestroyed;	//Number of good bots destroyed
	int badDestroyed;	//Number of bad bots destroyed
};