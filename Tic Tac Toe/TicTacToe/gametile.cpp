#include "gametile.h"


GameTile::GameTile()
{
	isMarked = 0.0f;
	circleCross = 0.0f;
}

GameTile::~GameTile()
{

}

void GameTile::Initialise()
{
	GLfloat quadVerts[] =
	{
		//--------------------------------------------- First Triangle (Left-bottom)
		// Position				 // Color			 //Tex Coords
		-0.24f, -0.24f, 0.0f,	 0.0f, 1.0f, 1.0f,	 0.0f, 1.0f,	//Left
		 0.24f, -0.24f, 0.0f,	 1.0f, 0.0f, 1.0f,	 1.0f, 1.0f,	//Right
		-0.24f,  0.24f, 0.0f,	 1.0f, 1.0f, 0.0f,	 0.0f, 0.0f,	//Top
		//--------------------------------------------- Second Triangle (Right-top)
		 0.24f,  0.24f, 0.0f,	 0.0f, 1.0f, 1.0f,	 1.0f, 0.0f		//Left
	};

	GLuint quadIndices[] = 
	{
		0, 1, 2, //1st Triangle
		3, 2, 1  //2nd Triangle
	};

	ShaderLoader shaderLoader;
	program = shaderLoader.CreateProgram("Assets/VertexShader.vs", "Assets/FragmentShader.fs", "vertShader0", "fragShader0");

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//EBO
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	//Culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//Textures
	int texW, texH, texChannels;

	//Texture 1
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	unsigned char* image = SOIL_load_image("Assets/CircleTile.png", &texW, &texH, &texChannels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Texture 2
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);

	unsigned char* image2 = SOIL_load_image("Assets/CrossTile.png", &texW, &texH, &texChannels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Texture 3
	glGenTextures(1, &tex3);
	glBindTexture(GL_TEXTURE_2D, tex3);

	unsigned char* image3 = SOIL_load_image("Assets/EmptyTile.png", &texW, &texH, &texChannels, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image3);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameTile::Draw()
{
	glUseProgram(program);

	GLfloat currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	currentTime = currentTime / 1000.0f;

	scrollU = sin(currentTime);
	scrollV = cos(currentTime);

	glUniform1i(glGetUniformLocation(program, "flipX"), 1);
	glUniform1i(glGetUniformLocation(program, "flipY"), 1);
	glUniform1f(glGetUniformLocation(program, "texU"), 0);
	glUniform1f(glGetUniformLocation(program, "texV"), 0);

	//Texture 1
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Texture 2
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Texture 3
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex3);
	glUniform1i(glGetUniformLocation(program, "tex3"), 2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Update position
	glUniform1f(glGetUniformLocation(program, "posX"), tileX);
	glUniform1f(glGetUniformLocation(program, "posY"), tileY);

	//Update circle or cross
	glUniform1f(glGetUniformLocation(program, "texMarked"), isMarked);
	glUniform1f(glGetUniformLocation(program, "texMix"), circleCross);

	//Debug Text
	//myGame->PrintText(0, 0.05f, std::to_string(arrayX));
	//myGame->PrintText(0, 0.0f, std::to_string(arrayY));
}

void GameTile::Update()
{
	//Syncronizing tile's display with game's array data
	if ((!myGame->GetCircleGrid(arrayX, arrayY)) && (!myGame->GetCrossGrid(arrayX, arrayY)))
	{
		isMarked = 0.0f;
	}
	else
	{
		if (myGame->GetCircleGrid(arrayX, arrayY)) //If it is circle
		{
			isMarked = 1.0f;
			circleCross = 0.0f;
		}
		else //If it is cross
		{
			isMarked = 1.0f;
			circleCross = 1.0f;
		}
	}

	realX = (int)((tileX + 0.5f)*400) + 200;
	realY = (int)((tileY - 0.5f)*-400) + 200;
	glutPostRedisplay();
}

void GameTile::CheckClick(int _x, int _y)
{
	int buffer = 90;

	if (myGame->GetGameState() == 0) //If game has not been won
	{
		if (_x >= realX - buffer && _x <= realX + buffer)
		{
			if (_y >= realY - buffer && _y <= realY + buffer)
			{
				if ((!myGame->GetCircleGrid(arrayX, arrayY)) && (!myGame->GetCrossGrid(arrayX, arrayY))) //If the tile is empty/unmarked
				{
					if (myGame->GetIsPlayerTurn()) //If it is the player one's turn
					{
						myGame->SetCircleGrid(arrayX, arrayY, true); //Mark a Circle in the spot
						myGame->SetCrossGrid(arrayX, arrayY, false);
						myGame->SetLastMove(arrayX, arrayY);
						myGame->UpdateAllGrid();
						myGame->SetMoveInputed(true);
						myGame->CheckWin(1);
					}
					else //If it is the player two's turn
					{
						if (!myGame->GetIsVersusAI()) //Versus another Human
						{
							myGame->SetCrossGrid(arrayX, arrayY, true); //Mark a Cross in the spot
							myGame->SetCircleGrid(arrayX, arrayY, false);
							myGame->SetLastMove(arrayX, arrayY);
							myGame->UpdateAllGrid();
							myGame->SetMoveInputed(true);
							myGame->CheckWin(2);
						}
					}
				}
			}
		}
	}
}

float GameTile::GetTileX() const
{
	return (tileX);
}

float GameTile::GetTileY() const
{
	return (tileY);
}

void GameTile::SetTileX( float _x)
{
	tileX = _x;
}

void GameTile::SetTileY(float _y)
{
	tileY = _y;
}

void GameTile::SetArrayX(int _x)
{
	arrayX = _x;
}

void GameTile::SetArrayY(int _y)
{
	arrayY = _y;
}

void GameTile::SetVBO(GLuint* _VBO)
{
	VBO = _VBO;
}

void GameTile::SetGame(Game* _game)
{
	myGame = _game;
}
