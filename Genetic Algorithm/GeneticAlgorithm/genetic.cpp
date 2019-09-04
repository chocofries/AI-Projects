#include "genetic.h"

Genetic::Genetic(Level* _level)
{
	level = _level;
	vecRobots = level->GetVecRobots();
	vecBombs = level->GetVecBombs();
	mutationChance = 10;
}

Genetic::~Genetic()
{
}

void Genetic::IterateGenerations() //Iterates through generations to find the most optimal bombing radius and location
{
	ShuffleBombs(); //Randomise new bombs

	int maxGenerations = 100;

	//Iterate through generations
	for (int g = 0; g < maxGenerations; g++)
	{
		//Sum all the bombs' scores in this generation
		int scoreSum = 0;
		std::vector<Bomb*>::iterator it;
		for (it = vecBombs->begin(); it != vecBombs->end(); it++)
		{
			Bomb* bomb = *it;
			bomb->CountCasualties();
			scoreSum += bomb->GetBombScore();
		}

		//Select Parents based on roulette selection
		parentA = RouletteSelect(scoreSum);
		parentB = RouletteSelect(scoreSum);

		//Spawn next generation of bombs based on parents
		SpawnNextGeneration();
	}

	//After iteration is done, find the bomb with the best score
	FindBestBomb();
}

void Genetic::SpawnNextGeneration() //Spawn next generation of bombs based on parents
{
	if (parentA == nullptr || parentB == nullptr)
	{
		std::cout << "ERROR: Parents not found." << std::endl;
		return;
	}

	//Get genetic information from parents
	float xA, xB, yA, yB, rA, rB;
	xA = parentA->GetX();
	xB = parentB->GetX();
	yA = parentA->GetY();
	yB = parentB->GetY();
	rA = parentA->GetBombRadius();
	rB = parentB->GetBombRadius();

	//Iterate through bombs and re-init with parental influence
	std::vector<Bomb*>::iterator it;
	for (it = vecBombs->begin(); it != vecBombs->end(); it++)
	{
		Bomb* bomb = *it;

		//Choose cross-over point
		int crossPoint = rand() % 6;

		//Mutation variation
		float muX = 0;
		float muY = 0;
		float muR = 0;

		if (rand() % 100 <= mutationChance)
		{
			muX = (float)((rand() % 256 - 128));
		}
		if (rand() % 100 <= mutationChance)
		{
			muY = (float)((rand() % 256 - 128));
		}

		if (rand() % 100 <= mutationChance)
		{
			muR = (float)((rand() % 100 - 50) * 0.01f);
		}

		switch (crossPoint)
		{
		case 0:
		{
			bomb->SetX(xB + muX);
			bomb->SetY(yA + muY);
			bomb->SetBombRadius(abs(rA + muR));
			break;
		}
		case 1:
		{
			bomb->SetX(xB + muX);
			bomb->SetY(yB + muY);
			bomb->SetBombRadius(abs(rA + muR));
			break;
		}
		case 2:
		{
			bomb->SetX(xB + muX);
			bomb->SetY(yA + muY);
			bomb->SetBombRadius(abs(rB + muR));
			break;
		}
		case 3:
		{
			bomb->SetX(xA + muX);
			bomb->SetY(yB + muY);
			bomb->SetBombRadius(abs(rB + muR));
			break;
		}
		case 4:
		{
			bomb->SetX(xA + muX);
			bomb->SetY(yA + muY);
			bomb->SetBombRadius(abs(rB + muR));
			break;
		}
		case 5:
		{
			bomb->SetX(xA + muX);
			bomb->SetY(yB + muY);
			bomb->SetBombRadius(abs(rA + muR));
			break;
		}
		}
	}
}

Bomb * Genetic::RouletteSelect(int _scoreSum) //Selects based on a roulette system
{
	std::vector<Bomb*>::iterator it;
	Bomb* parent = vecBombs->front();

	//Get roulette number
	int rouletteNum = rand() % (_scoreSum+1);

	//Decrement roulette number until 0 or less is reached
	while (rouletteNum > 0)
	{
		for (it = vecBombs->begin(); it != vecBombs->end(); it++)
		{
			Bomb* bomb = *it;
			bomb->CountCasualties();
			rouletteNum -= bomb->GetBombScore();

			if (rouletteNum <= 0)
			{
				parent = bomb;
				break;
			}
		}
	}

	return parent;
}

void Genetic::FindBestBomb() //Finds the bomb with the best bombscore (use only at the end of generation iteration)
{
	std::vector<Bomb*>::iterator it;
	bestBomb = vecBombs->front();
	int bestScore = -1000;

	for (it = vecBombs->begin(); it != vecBombs->end(); it++)
	{
		Bomb* bomb = *it;
		bomb->CountCasualties();
		if (bomb->GetBombScore() > bestScore)
		{
			bestScore = bomb->GetBombScore();
			bestBomb = bomb;
		}
		bomb->SetIsActive(false);
	}

	bestBomb->SetIsActive(true);
}

void Genetic::ShuffleRobots() //Shuffles the robots to different positions and alignments
{
	std::vector<Robot*>::iterator it;
	vecTargets.clear();

	for (it = vecRobots->begin(); it != vecRobots->end(); it++)
	{
		Robot* bot = *it;
		bot->SetX((float)((rand() % WINDOW_WIDTH*0.8f) - (WINDOW_WIDTH*0.4f)));
		bot->SetY((float)((rand() % WINDOW_HEIGHT*0.6f) - (WINDOW_HEIGHT*0.2f)));
		if (rand() % 100 <= badPercentage)
		{
			bot->SetIsGood(false);
			vecTargets.push_back(bot); //Push to vector of targets if bad robot
		}
		else
		{
			bot->SetIsGood(true);
		}
	}
}

void Genetic::ShuffleBombs() //Shuffles bombs to different targets and radii
{
	std::vector<Bomb*>::iterator it;

	for (it = vecBombs->begin(); it != vecBombs->end(); it++)
	{
		Robot* randomTarget = vecTargets.at(rand() % (vecTargets.size()));

		Bomb* bomb = *it;
		bomb->SetX((float)(randomTarget->GetX()));
		bomb->SetY((float)(randomTarget->GetY()));
		bomb->SetBombRadius((float)((rand()%75 + 5) * 0.01f));
		bomb->SetIsActive(false);
	}
}

void Genetic::UpdateAlignment()
{
	std::vector<Robot*>::iterator it;
	vecTargets.clear();

	for (it = vecRobots->begin(); it != vecRobots->end(); it++)
	{
		Robot* bot = *it;
		if (rand() % 100 <= badPercentage)
		{
			bot->SetIsGood(false);
			vecTargets.push_back(bot); //Push to vector of targets if bad robot
		}
		else
		{
			bot->SetIsGood(true);
		}
	}

	ShuffleBombs();
}

void Genetic::SetBadPercentage(int _percentage)
{
	badPercentage = _percentage;
}

Bomb * Genetic::GetBestBomb() const
{
	return bestBomb;
}
