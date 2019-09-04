#include "level.h"
#include "tile.h"

Level::Level()
{
	isInit = false;
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

	delete hillclimber;
	delete levelCamera;
}

void Level::Initialise(Game* _myGame, ShaderLoader* _shaderloader, TextureLoader* _textureloader, InputManager* _inputManager, int _rows, int _columns)
{
	shaderLoader = _shaderloader;
	textureLoader = _textureloader;
	inputManager = _inputManager;
	levelCamera = new Camera();

	glEnable(GL_TEXTURE_2D);
	glGenBuffers(1, &levelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, levelVBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Initialise Audio System
	//FMOD_RESULT result;
	//result = FMOD::System_Create(&audioSystem);
	//result = audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);

	//Init Text
	Texter* title = new Texter("SPACE: Find Brightest Star       UP/DOWN: Search Iterations       LEFT/RIGHT: Search Radius","Assets/Fonts/ComicSans.ttf", glm::vec3(-450.0f, -400.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	Texter* title2 = new Texter("LEFT MOUSE: Set Start Area       RIGHT MOUSE: Generate New Sky      MOUSE WHEEL: Change Grid Size", "Assets/Fonts/ComicSans.ttf", glm::vec3(-500.0f, 400.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	text1 = new Texter("Search Radius:", "Assets/Fonts/ComicSans.ttf", glm::vec3(-580.0f, 25.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	text2 = new Texter("Search Iterations:", "Assets/Fonts/ComicSans.ttf", glm::vec3(-580.0f, -25.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	title->SetScale(0.4f);
	title2->SetScale(0.4f);
	text1->SetScale(0.4f);
	text2->SetScale(0.4f);
	vecTexts.push_back(title);
	vecTexts.push_back(title2);
	vecTexts.push_back(text1);
	vecTexts.push_back(text2);

	//Init Pathfinder
	hillclimber = new Hillclimber();
	hillclimber->SetVecTiles(&vecObjects);
	pathFound = false;

	//Init Grid Of Tiles
	rows = _rows;
	columns = _columns;
	hillclimber->SetRows(rows);
	hillclimber->SetColumns(columns);
	float halfRow = (float)(rows-1) * 0.5f;
	float halfColumn = (float)(columns-1) * 0.5f;
	float tileScale = (float)((WINDOW_WIDTH+100)/columns);
	float gap = 1.0f;
	float offset = (tileScale*0.5f) + gap;
	float zoomOut = columns*0.55f;

	for (unsigned int i = 0; i < columns; i++)
	{
		for (unsigned int j = 0; j < rows; j++)
		{
			Tile* temp = new Tile();
			temp->SetVBO(&levelVBO);
			temp->SetCamera(levelCamera);
			temp->SetShaderLoader(shaderLoader);
			temp->SetInputManager(inputManager);
			temp->SetTextureLoader(textureLoader);
			temp->SetHillclimber(hillclimber);
			temp->SetImageFilePath("Assets/EmptyTile.png");
			temp->SetVecTexts(&vecTexts);
			temp->SetGridX(i);
			temp->SetGridY(j);
			temp->SetYRot(1.0f);
			temp->SetX((offset*(float)i) - (halfColumn * offset));
			temp->SetY((offset*-(float)j) + (halfRow * offset));
			temp->SetZ(0.0f);
			temp->SetXScale(tileScale);
			temp->SetYScale(tileScale);
			temp->LoadTheImage();
			temp->Initialise();
			temp->SetIsActive(true);
			vecObjects.push_back(temp);
			hillclimber->SetTileGrid(i, j, temp); //Add to grid of tiles in pathfinder

			if (i == 0 && j == 0) //Set as default start
			{
				temp->SetTileState(TILE_START);
				hillclimber->SetStartTile(temp);
			}
		}
	}
	isInit = true;
}

void Level::Update()
{
	//Update time
	GLfloat currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000.0f;

	//Camera
	levelCamera->ComputeView();
	levelCamera->ComputeProjection();

	//system("CLS");
	//std::cout << "x: " << screenMouseX << " | y: " << screenMouseY << std::endl;
	//std::cout << "Camera x: " << levelCamera->GetCamPos().x << " | y: " << levelCamera->GetCamPos().y << " | z: " << levelCamera->GetCamPos().z << std::endl;
	//std::cout << "Mouse State LB: " << inputManager->GetMouseState(0) << " | RB: " << inputManager->GetMouseState(2) << std::endl;
	
	ProcessKeyInput();

	//Update objects
	for (unsigned int i = 0; i < vecObjects.size(); i++)
	{
		if (vecObjects[i]->GetIsActive())
		{
			vecObjects[i]->Update();
		}
	}

	//Update text values
	std::string strRadius = std::to_string(hillclimber->GetSearchRadius());
	std::string strIterations = std::to_string(hillclimber->GetSearchIterations());
	const std::string str1 = "Search Radius: " + strRadius;
	const std::string str2 = "Search Iterations: " + strIterations;
	text1->SetText(str1);
	text2->SetText(str2);
}

void Level::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f); //clear

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
	if (!hillclimber->GetIsFinding())
	{
		if (inputManager->GetKeyState(' ') == KEY_DOWN)
		{
			hillclimber->Reset(false);
			hillclimber->Starfinder();
		}

		//Process search radius changes
		if (inputManager->GetArrowState(DIR_LEFT) == KEY_DOWN_BEGIN)
		{
			int prev = hillclimber->GetSearchRadius();

			if (prev - 1 >= 1)
			{
				hillclimber->SetSearchRadius(prev - 1);
			}
		}
		else if (inputManager->GetArrowState(DIR_RIGHT) == KEY_DOWN_BEGIN)
		{
			int prev = hillclimber->GetSearchRadius();

			if (prev + 1 <= 10)
			{
				hillclimber->SetSearchRadius(prev + 1);
			}
		}

		//Process search iteration changes
		if (inputManager->GetArrowState(DIR_DOWN) == KEY_DOWN_BEGIN)
		{
			int prev = hillclimber->GetSearchIterations();

			if (prev - 1 >= 1)
			{
				hillclimber->SetSearchIterations(prev - 1);
			}
		}
		else if (inputManager->GetArrowState(DIR_UP) == KEY_DOWN_BEGIN)
		{
			int prev = hillclimber->GetSearchIterations();

			if (prev + 1 <= 10)
			{
				hillclimber->SetSearchIterations(prev + 1);
			}
		}

		//Process grid size changes
		if (hillclimber->GetIsResizing() == false)
		{
			int prevSize = hillclimber->GetRows();

			if (inputManager->GetMouseWheelDir() == -1 && prevSize - 1 >= 3)
			{
				hillclimber->Reset(true);
				hillclimber->ResizeGrid(prevSize - 1, prevSize - 1);
			}

			if (inputManager->GetMouseWheelDir() == 1 && prevSize + 1 <= GRID_MAX_ROWS)
			{
				hillclimber->Reset(true);
				hillclimber->ResizeGrid(prevSize + 1, prevSize + 1);
			}
		}
	}
}

std::vector<Texter*>* Level::GetVecTexts()
{
	return &vecTexts;
}

Hillclimber * Level::GetPathfinder()
{
	return hillclimber;
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
