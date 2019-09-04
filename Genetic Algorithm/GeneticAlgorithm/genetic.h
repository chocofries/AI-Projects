/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	genetic.h
Description	:	Genetic Algorithm to find best bombing position and radius
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/

#pragma once
#include "level.h"
#include "bomb.h"
#include "robot.h"

class Genetic
{
public:
	Genetic(Level* _level);
	~Genetic();

	void IterateGenerations();
	void SpawnNextGeneration();
	Bomb* RouletteSelect(int _scoreSum);

	void FindBestBomb();
	void ShuffleRobots();
	void ShuffleBombs();
	void UpdateAlignment(); //Updates good/bad alignment of robots based on badPercentage

	void SetBadPercentage(int _percentage);
	Bomb* GetBestBomb() const;

private:
	int badPercentage;
	int mutationChance; //Percentage of chance to mutate

	Level * level;
	std::vector<Robot*>* vecRobots;
	std::vector<Robot*> vecTargets;
	std::vector<Bomb*>* vecBombs;

	Bomb* parentA;
	Bomb* parentB;
	Bomb * bestBomb;
};