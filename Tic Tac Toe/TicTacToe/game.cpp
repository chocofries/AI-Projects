#include "game.h"


Game::Game()
{
	gameState = 0;
	gameDifficulty = 1;
	turnNumber = 0;

	startPlayer = true;
	isPlayerTurn = startPlayer;
	moveInputed = false;
	isVersusAI = true;

	mouseX = 0;
	mouseY = 0;
}

Game::~Game()
{
	while (vecTiles.size() > 0)
	{
		GameTile* tempTile = vecTiles[vecTiles.size() - 1];
		vecTiles.pop_back();
		delete tempTile;
	}
}

void Game::Initialise()
{
	glEnable(GL_TEXTURE_2D);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			GameTile* tempTile = new GameTile();
			tempTile->SetVBO(&VBO);
			tempTile->SetGame(this);
			tempTile->Initialise();
			tempTile->SetArrayX(j);
			tempTile->SetArrayY(2-i);
			tempTile->SetTileX((j*0.5f) - 0.5f);
			tempTile->SetTileY((i*0.5f) - 0.5f);
			vecTiles.push_back(tempTile);
		}
	}
	srand((unsigned int)time(NULL));
}

void Game::PrintText(float _x, float _y, std::string _string)
{
	glRasterPos2f(_x, _y);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.8f, 0.0f, 0.8f);
	glDisable(GL_LIGHTING);
	for (unsigned int i = 0; i < _string.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, _string[i]);
	}
}

void Game::Update()
{
	//Update tiles
	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		vecTiles[i]->Update();
	}

	//Artificial Intelligence Turn
	if (moveInputed)
	{
		UpdateAllGrid();
		NextPlayerTurn();
		moveInputed = false;
	}

	if ((!isPlayerTurn) && (isVersusAI) && gameState == 0)
	{
		AI();
	}

	std::cout << mouseX << ", " << mouseY << std::endl;
	std::cout << "Turn Number: " << turnNumber << std::endl;
	std::cout << "Difficulty (Change with 'D'): ";
	if (gameDifficulty == 0)
	{
		std::cout << "Easy" << std::endl;
	}
	else
	{
		std::cout << "Unbeatable" << std::endl;
	}

	if (gameState == 1)
	{
		std::cout << "Circle Wins!" << std::endl;
		std::cout << "Right click to play again!" << std::endl;
	}
	if (gameState == 2)
	{
		std::cout << "Cross Wins!" << std::endl;
		std::cout << "Right click to play again!" << std::endl;
	}
	if (gameState == 3)
	{
		std::cout << "It's a Draw!" << std::endl;
		std::cout << "Right click to play again!" << std::endl;
	}
	system("CLS");

	UpdateAllGrid();
}

void Game::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	for (unsigned int i = 0; i < vecTiles.size(); i++)
	{
		vecTiles[i]->Draw();
	}

	glutSwapBuffers();
}

void Game::MouseMove(int _x, int _y)
{
	mouseX = _x;
	mouseY = _y;
}

void Game::MouseClick(int _button, int _state, int _x, int _y)
{
	int clickX = _x;
	int clickY = _y;

	static bool hasClicked = false;

	if (_button == GLUT_LEFT_BUTTON)
	{
		if (_state == GLUT_DOWN)
		{
			for (unsigned int i = 0; i < vecTiles.size(); i++)
			{
				vecTiles[i]->CheckClick(clickX, clickY);
			}
			hasClicked = true;
		}
		if (_state == GLUT_UP)
		{
			hasClicked = false;
		}
	}
	if (_button == GLUT_RIGHT_BUTTON)
	{
		if (_state == GLUT_UP && gameState != 0)
		{
			GameRestart();
		}
	}
}

void Game::KeyPress(unsigned char _key, int _x, int _y)
{
	int clickX = _x;
	int clickY = _y;

	if (_key == 'r')
	{
		GameRestart();
	}
	if (_key == 'd')
	{
		if (gameDifficulty == 0)
		{
			gameDifficulty = 1;
		}
		else
		{
			gameDifficulty = 0;
		}
	}
}

void Game::NextPlayerTurn()
{
	if (gameState == 0)
	{
		if (isPlayerTurn)
		{
			isPlayerTurn = false;
		}
		else
		{
			isPlayerTurn = true;
		}
	}
	turnNumber++;
}

void Game::AI()
{
	int aX, aY;
	bool turnEnd = false;

	if (turnNumber >= 2)
	{
		//Do something
		int iTemp = 0;
	}

	if (gameDifficulty == 0) //Easy Difficulty
	{
		//Generate Random Number
		aX = rand() % 3;
		aY = rand() % 3;

		while (!turnEnd)
		{
			if ((!circleGrid[aX][aY]) && (!crossGrid[aX][aY])) //Tile is unmarked
			{
				crossGrid[aX][aY] = true;
				circleGrid[aX][aY] = false;
				lastX = aX;
				lastY = aY;
				UpdateAllGrid();
				turnEnd = true;
				break;
			}
			else
			{
				aX = rand() % 3;
				aY = rand() % 3;
			}
		}
	}
	else if (gameDifficulty == 1) //Hard Difficulty
	{
		Minimax* tempMinimax = new Minimax(this); //Create new minimax tree

		tempMinimax->BestMove(); //Find the best move for this turn
		aX = tempMinimax->GetBestX();
		aY = tempMinimax->GetBestY();

		delete tempMinimax;

		//Play the most optimal move
		crossGrid[aX][aY] = true;
		circleGrid[aX][aY] = false;
		lastX = aX;
		lastY = aY;
		UpdateAllGrid();
		turnEnd = true;
	}
	
	CheckWin(2); //Check if cross wins
	NextPlayerTurn();
}

void Game::GameRestart()
{
	gameState = 0;
	isPlayerTurn = startPlayer;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			circleGrid[j][i] = false;
			crossGrid[j][i] = false;
		}
	}
	UpdateAllGrid();
	turnNumber = 0;
}

void Game::Sleep(int _msecs)
{
	GLfloat timeP = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	float tick = 0;

	while (tick < (float)_msecs)
	{
		GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
		GLfloat timeDiff = time - timeP;
		timeP = time;

		tick += timeDiff;
	}
}

void Game::UpdateAllGrid()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (circleGrid[j][i] == true)
			{
				allGrid[j][i] = 1;
			}
			else if(crossGrid[j][i] == true)
			{
				allGrid[j][i] = 2;
			}
			else
			{
				allGrid[j][i] = 0;
			}
		}
	}
}

bool Game::CheckWin(int _player)
{
	int filledSpaces = 0;

	for (int v = 0; v < 3; v++)
	{
		for (int h = 0; h < 3; h++)
		{
			//Horizontals and verticals
			if ((allGrid[0][v] == _player && allGrid[1][v] == _player && allGrid[2][v] == _player) ||
				(allGrid[h][0] == _player && allGrid[h][1] == _player && allGrid[h][2] == _player))
			{
				gameState = _player;
				return true;
			}

			//Number of filled spaces
			if (allGrid[h][v] != 0)
			{
				filledSpaces += 1;
			}
		}
	}

	//Diagonals
	if ((allGrid[0][0] == _player && allGrid[1][1] == _player && allGrid[2][2] == _player) ||
		(allGrid[0][2] == _player && allGrid[1][1] == _player && allGrid[2][0] == _player))
	{
		gameState = _player;
		return true;
	}

	//If all spaces are filled, and neither player won, then set as draw
	if (filledSpaces == 9)
	{
		gameState = 3;
	}

	return false;
}

void Game::SetCircleGrid(int _x, int _y, bool _set)
{
	circleGrid[_x][_y] = _set;
}

void Game::SetCrossGrid(int _x, int _y, bool _set)
{
	crossGrid[_x][_y] = _set;
}

void Game::SetLastMove(int _x, int _y)
{
	lastX = _x;
	lastY = _y;
}

void Game::SetMoveInputed(bool _inputed)
{
	moveInputed = _inputed;
}

bool Game::GetCircleGrid(int _x, int _y) const
{
	bool temp = circleGrid[_x][_y];
	return temp;
}

bool Game::GetCrossGrid(int _x, int _y) const
{
	bool temp = crossGrid[_x][_y];
	return temp;
}

bool Game::GetIsPlayerTurn() const
{  
	return isPlayerTurn;
}

bool Game::GetIsVersusAI() const
{
	return isVersusAI;
}

bool Game::GetGameState() const
{
	return gameState;
}

bool Game::GetMoveInputed() const
{
	return moveInputed;
}

int Game::GetAllGrid(int _x, int _y) const
{
	return allGrid[_x][_y];
}

int Game::GetTurnNumber() const
{
	return turnNumber;
}

int Game::GetLastX() const
{
	return lastX;
}

int Game::GetLastY() const
{
	return lastY;
}
