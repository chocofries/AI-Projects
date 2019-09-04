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

	delete pathfinder;
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
	Texter* title = new Texter("A: Find Path       S: Choose Start       D: Choose Destination","Assets/Fonts/ComicSans.ttf", glm::vec3(-300.0f, -400.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	Texter* title2 = new Texter("LEFT MOUSE: Add Obstacle       RIGHT MOUSE: Remove Obstacle      MOUSE WHEEL: Change Grid Size", "Assets/Fonts/ComicSans.ttf", glm::vec3(-500.0f, 400.0f, 2.0f), shaderLoader, textureLoader, levelCamera);
	title->SetScale(0.5f);
	title2->SetScale(0.4f);
	vecTexts.push_back(title);
	vecTexts.push_back(title2);

	//Init Pathfinder
	pathfinder = new Pathfinder();
	pathfinder->SetVecTiles(&vecObjects);
	pathFound = false;

	//Init Grid Of Tiles
	rows = _rows;
	columns = _columns;
	pathfinder->SetRows(rows);
	pathfinder->SetColumns(columns);
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
			temp->SetPathfinder(pathfinder);
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
			pathfinder->SetTileGrid(i, j, temp); //Add to grid of tiles in pathfinder

			if (i == 0 && j == 0) //Set as default start
			{
				temp->SetTileState(TILE_START);
				pathfinder->SetStartTile(temp);
			}

			if (i == columns-1 && j == rows-1) //Set as default destination
			{
				temp->SetTileState(TILE_DEST);
				pathfinder->SetDestTile(temp);
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
}

void Level::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //clear

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
	if (!pathfinder->GetIsFinding())
	{
		if (inputManager->GetKeyState('a') == KEY_DOWN || inputManager->GetKeyState('A') == KEY_DOWN)
		{
			pathfinder->Reset(false);
			pathfinder->Starfinder();
		}

		if (inputManager->GetKeyState(' ') == KEY_DOWN)
		{
			pathfinder->Reset(true);
		}

		//Process grid size changes
		if (pathfinder->GetIsResizing() == false)
		{
			int prevSize = pathfinder->GetRows();

			if (inputManager->GetMouseWheelDir() == -1 && prevSize - 1 >= 3)
			{
				pathfinder->Reset(true);
				pathfinder->ResizeGrid(prevSize - 1, prevSize - 1);
			}

			if (inputManager->GetMouseWheelDir() == 1 && prevSize + 1 <= GRID_MAX_ROWS)
			{
				pathfinder->Reset(true);
				pathfinder->ResizeGrid(prevSize + 1, prevSize + 1);
			}
		}
	}
}

std::vector<Texter*>* Level::GetVecTexts()
{
	return &vecTexts;
}

Pathfinder * Level::GetPathfinder()
{
	return pathfinder;
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

void Level::CalculateRaycast()
{
	float rx, ry, rz;

	//Raycasting from screen-space mouse position
	rx = (2.0f * (float)screenMouseX) / (float)WINDOW_WIDTH - 1.0f;
	ry = 1.0f - (2.0f * (float)screenMouseY) / (float)WINDOW_HEIGHT;
	rz = 1.0f;

	glm::vec3 rayNDS = glm::vec3(rx, ry, rz); //3D normalised device coordinates

	glm::vec4 rayClip = glm::vec4(rayNDS.x,rayNDS.y,-1.0f,1.0f); //4D homogenous clip coordinates

	glm::vec4 rayEye = glm::inverse(levelCamera->GetProjection()) * rayClip; //4D eye (camera) coordinates
	rayEye = glm::vec4(rayEye.x,rayEye.y,-1.0f,0.0f);

	rayWorld = glm::inverse(levelCamera->GetView()) * rayEye; //4D world coordinates
	rayWorld = glm::normalize(rayWorld); //Normalize

	glm::vec3 rayOrigin = glm::vec3(rayEye.x, rayEye.y, rayEye.z);
	
	glm::vec3 planeNormal = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 planeOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

	bool isIntersect = false;

	float denominator = glm::dot(rayWorld,planeNormal);
	if (abs(denominator) > 0.01f)
	{
		float dist = glm::distance(planeOrigin, rayOrigin);
		float numerator = glm::dot(rayOrigin, planeNormal) + dist;
		float t = numerator / denominator;

		if (t > 0)
		{
			isIntersect = true;
			intersection = (rayOrigin + (rayWorld * t)); //Point of intersection on plane
		}
	}
}
