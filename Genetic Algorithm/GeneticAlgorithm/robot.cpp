#include "robot.h"
#include "level.h"

Robot::Robot(Level* _level)
{
	level = _level;
	VBO = level->GetVBO();
	gameCamera = level->GetCamera();
	inputManager = level->GetInputManager();
	shaderLoader = level->GetShaderLoader();
	textureLoader = level->GetTextureLoader();
	vecTexts = level->GetVecTexts();
	clock = level->GetClock();
}

Robot::~Robot()
{
	delete sprites[GOOD_BOT];
	delete sprites[BAD_BOT];
}

void Robot::Initialise()
{
	sprites[GOOD_BOT] = new Sprite(this);
	sprites[GOOD_BOT]->InitSprite("Assets/GoodBot.png","GoodBot",0.0f,1,1,1,0,128,128);
	sprites[GOOD_BOT]->SetSpriteScale(0.5f);

	sprites[BAD_BOT] = new Sprite(this);
	sprites[BAD_BOT]->InitSprite("Assets/BadBot.png", "BadBot", 0.0f, 1, 1, 1, 0, 128, 128);
	sprites[BAD_BOT]->SetSpriteScale(0.5f);

	currentSprite = sprites[GOOD_BOT];
}

void Robot::Update(double _dTime)
{
	//Check alignment
	if (isGood)
	{
		currentSprite = sprites[GOOD_BOT];
	}
	else
	{
		currentSprite = sprites[BAD_BOT];
	}

	deltaTime = _dTime;
	currentSprite->Update(deltaTime);
}

void Robot::Draw()
{
	currentSprite->Draw();
}

void Robot::SetIsGood(bool _isGood)
{
	isGood = _isGood;
}

bool Robot::GetIsGood() const
{
	return isGood;
}
