#include "bomb.h"
#include "level.h"

Bomb::Bomb(Level * _level)
{
	level = _level;
	VBO = level->GetVBO();
	gameCamera = level->GetCamera();
	inputManager = level->GetInputManager();
	shaderLoader = level->GetShaderLoader();
	textureLoader = level->GetTextureLoader();
	vecTexts = level->GetVecTexts();
	vecRobots = level->GetVecRobots();
	clock = level->GetClock();
	casualties = 0;
	bombScore = 0;
	bombRadius = 1.0f;
	badDestroyed = 0;
	goodDestroyed = 0;
	casualties = 0;
}

Bomb::~Bomb()
{
	delete sprite;
}

void Bomb::Initialise()
{
	sprite = new Sprite(this);
	sprite->InitSprite("Assets/BombRadius.png", "BombRadius", 0.0f, 1, 1, 1, 0, 512, 512);
	bombRadius = (float)(rand() % 100)*0.01f;
}

void Bomb::Update(double _dTime)
{
	deltaTime = _dTime;
	sprite->SetSpriteScale(bombRadius * 2.0f);
	sprite->Update(deltaTime);
}

void Bomb::Draw()
{
	sprite->Draw();
}

void Bomb::SetBombRadius(float _radius)
{
	bombRadius = _radius;
}

void Bomb::CountCasualties()
{
	std::vector<Robot*>::iterator it;

	float buffer = 5.0f;
	goodDestroyed = 0;
	badDestroyed = 0;

	for (it = vecRobots->begin(); it != vecRobots->end(); it++)
	{
		Robot* bot = *it;

		//Check if in range of the bomb
		float distance = Utils::FindDistance(x,y,bot->GetX(),bot->GetY());
		if (distance + buffer <= bombRadius * 256.0f)
		{
			//Count the casualties
			if (bot->GetIsGood())
			{
				goodDestroyed++;
			}
			else
			{
				badDestroyed++;
			}
		}
	}

	casualties = badDestroyed + goodDestroyed;
	bombScore = (int)(1 * (badDestroyed/((goodDestroyed*3)+1)));
}

int Bomb::GetBombScore() const
{
	return bombScore;
}

float Bomb::GetBombRadius() const
{
	return bombRadius;
}

int Bomb::GetGoodDestroyed() const
{
	return goodDestroyed;
}

int Bomb::GetBadDestroyed() const
{
	return badDestroyed;
}

int Bomb::GetCasualties() const
{
	return casualties;
}
