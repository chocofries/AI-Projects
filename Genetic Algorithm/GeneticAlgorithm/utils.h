/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	utils.h
Description	:	Miscellaneous utilities and definitions
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/
#pragma once

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

class Utils
{
public:
	static float FindDistance(float x1, float y1, float x2, float y2)
	{
		float distance = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
		return distance;
	}
};