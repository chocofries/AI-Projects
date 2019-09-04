#include "tile.h"
#include "pathfinder.h"

Tile::Tile()
{
}

Tile::~Tile()
{
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VAO);
}

void Tile::Initialise()
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

	program = shaderLoader->CreateProgram("Assets/VertexShader.vs", "Assets/FragmentShader.fs", "vertShader0", "fragShader0");

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	//Vertex Attributes
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//Get Uniform Locations
	fillLoc = glGetUniformLocation(program, "fillColor");
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projLoc = glGetUniformLocation(program, "proj");
	texLoc = glGetUniformLocation(program, "tex");

	//Init Text
	textF = new Texter("F", "Assets/Fonts/arial.ttf", glm::vec3(x - 0.0f, y + 0.0f, z + 1.0f), shaderLoader, textureLoader, gameCamera, false);
	textF->SetScale(32.0f);
	vecTexts->push_back(textF);

	textG = new Texter("G", "Assets/Fonts/arial.ttf", glm::vec3(x - 15.0f, y - 15.0f, z + 1.0f), shaderLoader, textureLoader, gameCamera, false);
	textG->SetScale(20.0f);
	vecTexts->push_back(textG);

	textH = new Texter("H", "Assets/Fonts/arial.ttf", glm::vec3(x + 10.0f, y - 15.0f, z + 1.0f), shaderLoader, textureLoader, gameCamera, false);
	textH->SetScale(20.0f);
	vecTexts->push_back(textH);

	//Tile State
	tileState = TILE_FREE;
}

void Tile::Update()
{
	//Position
	glm::vec3 objPosition = glm::vec3(x, y, z);
	
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), objPosition);

	//Rotation
	glm::vec3 rotationAxisZ = glm::vec3(xRot, yRot, zRot);
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxisZ);

	//Scale
	glm::vec3 objScale = glm::vec3(xScale, yScale, zScale);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), objScale);

	////Model Matrix
	model = translationMatrix * rotationZ * scaleMatrix;

	////View Matrix
	view = gameCamera->GetView();

	////Projection Matrix
	proj = gameCamera->GetProjection();

	//Text
	float halfWidth = ((float)WINDOW_WIDTH * 0.5f);
	float halfHeight = ((float)WINDOW_HEIGHT * 0.5f);

	screenX = (int)(x + halfWidth);
	screenY = (int)-(y - halfHeight);

	std::string strF = std::to_string(scoreF);
	std::string strG = std::to_string(scoreG);
	std::string strH = std::to_string(scoreH);
	textF->SetText(strF);
	textG->SetText(strG);
	textH->SetText(strH);
	textF->SetPosition(glm::vec3(x - 10.0f, y - 5.0f, z + 1.0f));

	//Process Mouse Clicks
	ProcessInput();

	//Fill Colour based on state
	switch (tileState)
	{
	case TILE_FREE:
	{
		fillColor = glm::vec3(0.4f, 0.4f, 0.4f); //Light Gray
		textF->SetIsActive(false);
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_START:
	{
		fillColor = glm::vec3(0.0f, 0.8f, 0.0f); //Green
		textF->SetIsActive(false);
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_DEST:
	{
		fillColor = glm::vec3(0.8f, 0.0f, 0.0f); //Red
		textF->SetIsActive(false);
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_OBS:
	{
		fillColor = glm::vec3(0.2f, 0.2f, 0.2f); //Dark Gray
		textF->SetIsActive(false);
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_OPEN:
	{
		fillColor = glm::vec3(0.2f, 0.4f, 0.6f); //Light Blue
		if (xScale >= 80.0f)
		{
			textF->SetIsActive(true);
		}
		else
		{
			textF->SetIsActive(false);
		}
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_CLOSED:
	{
		fillColor = glm::vec3(0.1f, 0.2f, 0.4f); //Blue
		if (xScale >= 80.0f)
		{
			textF->SetIsActive(true);
		}
		else
		{
			textF->SetIsActive(false);
		}
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	case TILE_PATH:
	{
		fillColor = glm::vec3(0.8f, 0.6f, 0.0f); //Amber/Gold
		if (xScale >= 80.0f)
		{
			textF->SetIsActive(true);
		}
		else
		{
			textF->SetIsActive(false);
		}
		textG->SetIsActive(false);
		textH->SetIsActive(false);
		break;
	}
	}

	glutPostRedisplay();
}

void Tile::Draw()
{
	glUseProgram(program);

	glUniform3fv(fillLoc, 1, glm::value_ptr(fillColor));

	//Model Matrix
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//View Matrix
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Projection Matrix
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texLoc, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Drawing based on verts
	glBindVertexArray(VAO);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tile::ProcessInput()
{
	int half = (int)(xScale*0.25f);
	int mx = inputManager->GetMouseX();
	int my = inputManager->GetMouseY();


	if (tileState != TILE_START && tileState != TILE_DEST && pathfinder->GetIsFinding() == false)
	{
		if (inputManager->GetMouseState(MOUSE_LEFT) == KEY_DOWN || inputManager->GetMouseState(MOUSE_RIGHT) == KEY_DOWN)
		{
			pathfinder->Reset(false);
			if (mx > screenX - half && mx < screenX + half)
			{
				if (my > screenY - half && my < screenY + half)
				{
					if (inputManager->GetMouseState(MOUSE_LEFT) == KEY_DOWN)
					{
						tileState = TILE_OBS;
					}
					if (inputManager->GetMouseState(MOUSE_RIGHT) == KEY_DOWN)
					{
						tileState = TILE_FREE;
					}
				}
			}
		}

		if (inputManager->GetKeyState('s') == KEY_DOWN || inputManager->GetKeyState('S') == KEY_DOWN)
		{
			if (tileState == TILE_FREE)
			{
				pathfinder->Reset(false);
				if (mx > screenX - half && mx < screenX + half)
				{
					if (my > screenY - half && my < screenY + half)
					{
						tileState = TILE_START;
						Tile* temp = pathfinder->GetStartTile();
						temp->SetTileState(TILE_FREE);
						pathfinder->SetStartTile(this);
					}
				}
			}
		}

		if (inputManager->GetKeyState('d') == KEY_DOWN || inputManager->GetKeyState('D') == KEY_DOWN)
		{
			pathfinder->Reset(false);
			if (tileState == TILE_FREE)
			{
				if (mx > screenX - half && mx < screenX + half)
				{
					if (my > screenY - half && my < screenY + half)
					{
						tileState = TILE_DEST;
						Tile* temp = pathfinder->GetDestTile();
						temp->SetTileState(TILE_FREE);
						pathfinder->SetDestTile(this);
					}
				}
			}
		}
	}


}

TileState Tile::GetTileState() const
{
	return tileState;
}

int Tile::GetScoreF() const
{
	return scoreF;
}

int Tile::GetScoreG() const
{
	return scoreG;
}

int Tile::GetScoreH() const
{
	return scoreH;
}

int Tile::GetGridX() const
{
	return gridX;
}

int Tile::GetGridY() const
{
	return gridY;
}

Tile * Tile::GetParentTile()
{
	return parentTile;
}

void Tile::SetPathfinder(Pathfinder * p)
{
	pathfinder = p;
}

void Tile::SetTileState(TileState _state)
{
	tileState = _state;
}

void Tile::SetParentTile(Tile * pt)
{
	parentTile = pt;
}

void Tile::SetRayOrigin(glm::vec3 * rO)
{
	rayOrigin = rO;
}

void Tile::SetRayWorld(glm::vec3 * rW)
{
	rayWorld = rW;
}

void Tile::SetScoreF(int _F)
{
	scoreF = _F;
}

void Tile::SetScoreG(int _G)
{
	scoreG = _G;
}

void Tile::SetScoreH(int _H)
{
	scoreH = _H;
}

void Tile::SetGridX(int _x)
{
	gridX = _x;
}

void Tile::SetGridY(int _y)
{
	gridY = _y;
}

void Tile::SetTextActive(bool isText)
{
	if (isText)
	{
		textF->SetIsActive(true);
	}
	else
	{
		textF->SetIsActive(false);
	}
}
