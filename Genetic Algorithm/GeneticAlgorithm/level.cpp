#include "level.h"
#include "robot.h"
#include "bomb.h"
#include "genetic.h"

Level::Level()
{
	isInit = false;
	badPercentage = 75;
}

Level::~Level()
{
	while (vecObjects.size() > 0)
	{
		GameObject* temp = vecObjects[vecObjects.size() - 1];
		vecObjects.pop_back();
		delete temp;
	}

	while (vecTexts.size() > 0)
	{
		Texter* temp = vecTexts[vecTexts.size() - 1];
		vecTexts.pop_back();
		delete temp;
	}
	delete levelCamera;
	delete clock;
	delete genetic;
}

void Level::Initialise(Game* _myGame, ShaderLoader* _shaderloader, TextureLoader* _textureloader, InputManager* _inputManager)
{
	shaderLoader = _shaderloader;
	textureLoader = _textureloader;
	inputManager = _inputManager;
	levelCamera = new Camera();

	glEnable(GL_TEXTURE_2D);
	glGenBuffers(1, &levelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, levelVBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Init Clock
	clock = new Clock();
	clock->Initialise();

	//Init Genetic Algorithm
	genetic = new Genetic(this);
	genetic->SetBadPercentage(badPercentage);

	//Init Robots
	for (int i = 0; i < 200; i++)
	{
		Robot* tempBot = new Robot(this);
		tempBot->SetX((float)((rand() % WINDOW_WIDTH*0.8f) - (WINDOW_WIDTH*0.4f)));
		tempBot->SetY((float)((rand() % WINDOW_HEIGHT*0.6f) - (WINDOW_HEIGHT*0.2f)));
		tempBot->SetZ((float)i*0.01f);
		tempBot->Initialise();
		tempBot->SetIsActive(true);
		vecObjects.push_back(tempBot);
		vecRobots.push_back(tempBot);
	}

	genetic->ShuffleRobots();

	//Init Bombs
	for (int i = 0; i < 50; i++)
	{
		Bomb* tempBomb = new Bomb(this);
		tempBomb->SetZ((float)i*0.1f + 5.0f);
		tempBomb->Initialise();
		tempBomb->SetIsActive(false);
		vecObjects.push_back(tempBomb);
		vecBombs.push_back(tempBomb);
	}

	genetic->ShuffleBombs();

	//Init Text
	textA = new Texter("Genetic Algorithm - Collateral Damage", "Assets/Fonts/arial.ttf", glm::vec3(-500.0f, -420.0f, 5.0f), shaderLoader, textureLoader, levelCamera, true, glm::vec3(0.8f, 0.8f, 0.8f), 0.5f);
	vecTexts.push_back(textA);

	isInit = true;
}

void Level::Update()
{
	//Update time
	clock->Process();
	double deltaTime = clock->GetDeltaTick();

	//Genetic
	genetic->SetBadPercentage(badPercentage);

	//Camera
	levelCamera->ComputeView();
	levelCamera->ComputeProjection();

	ProcessKeyInput();

	//Update objects
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		if (vecObjects[i]->GetIsActive())
		{
			vecObjects[i]->Update(deltaTime);
		}
	}

	//Update Text
	if (genetic->GetBestBomb() != nullptr)
	{
		std::string str = "Bad Bots Destroyed: ";
		str += std::to_string(genetic->GetBestBomb()->GetBadDestroyed());
		str += "          Good Bots Destroyed: ";
		str += std::to_string(genetic->GetBestBomb()->GetGoodDestroyed());
		str += "          Evilness: ";
		str += std::to_string(badPercentage);
		str += "%";
		textA->SetText(str);
	}
	else
	{
		std::string str = "Evilness: ";
		str += std::to_string(badPercentage);
		str += "%";
		textA->SetText(str);
	}

	glutPostRedisplay();
}

void Level::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); //clear

	//Draw objects
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		if (vecObjects[i]->GetIsActive())
		{
			vecObjects[i]->Draw();
		}
	}

	//Draw texts
	for (unsigned int i = 0; i < vecTexts.size(); i++)
	{
		vecTexts[i]->Render();
	}

	glutSwapBuffers();
}

void Level::ProcessKeyInput()
{
	if (inputManager->GetMouseState(MOUSE_LEFT) == KEY_DOWN)
	{
		genetic->IterateGenerations();
	}
	else if (inputManager->GetMouseState(MOUSE_RIGHT) == KEY_DOWN)
	{
		genetic->ShuffleRobots();
		genetic->ShuffleBombs();
	}

	if (inputManager->GetMouseWheelDir() == 1)
	{
		if (badPercentage < 99)
		{
			badPercentage += 1;
			genetic->UpdateAlignment();
		}
	}
	else if(inputManager->GetMouseWheelDir() == -1)
	{
		if (badPercentage > 1)
		{
			badPercentage -= 1;
			genetic->UpdateAlignment();
		}
	}
}

std::vector<Texter*>* Level::GetVecTexts()
{
	return &vecTexts;
}

std::vector<Robot*>* Level::GetVecRobots()
{
	return &vecRobots;
}

std::vector<Bomb*>* Level::GetVecBombs()
{
	return &vecBombs;
}

GLuint Level::GetVBO() const
{
	return levelVBO;
}

ShaderLoader * Level::GetShaderLoader() const
{
	return shaderLoader;
}

TextureLoader * Level::GetTextureLoader() const
{
	return textureLoader;
}

InputManager * Level::GetInputManager() const
{
	return inputManager;
}

Camera * Level::GetCamera() const
{
	return levelCamera;
}

Clock * Level::GetClock() const
{
	return clock;
}

bool Level::GetIsInit() const
{
	return isInit;
}

void Level::SetScreenMouseX(int _x)
{
	screenMouseX = _x;
}


void Level::SetScreenMouseY(int _y)
{
	screenMouseY = _y;
}
