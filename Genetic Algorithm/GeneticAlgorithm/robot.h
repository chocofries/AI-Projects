/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	robot.h
Description	:	Robot object, can be good or bad
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/

#pragma once
#include "sprite.h"

class Robot : public GameObject
{
public:
	Robot(Level* _level);
	virtual ~Robot();

	virtual void Initialise();
	virtual void Update(double _dTime);
	virtual void Draw();

	void SetIsGood(bool _isGood);
	bool GetIsGood() const;

private:
	enum robotAlignment
	{
		GOOD_BOT,
		BAD_BOT
	};
	Sprite * sprites[2];
	Sprite * currentSprite;
	bool isGood;
};