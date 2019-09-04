/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	clock.h
Description	:	System to keep accurate time and calculate deltatime
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/

#pragma once
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cstdlib>

class Clock
{
public:
	Clock();
	~Clock();

	bool Initialise();

	void Process();

	double GetDeltaTick();
	int GetMsPassed() const;

protected:
	double timeElapsed;
	double deltaTime;
	double lastTime;
	double currentTime;
	double secondsPerCount;
	int frameCount = 0;

	clock_t startTime;
	double msPassed;

};
