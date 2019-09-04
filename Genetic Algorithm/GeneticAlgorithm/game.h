/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	game.h
Description	:	System to manage a game process
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/

#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fmod.hpp>
#include "shaderloader.h"
#include "textureloader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "level.h"
#include "inputmanager.h"

class Level;
class InputManager;

class Game
{
public:
	Game();
	~Game();

	void Initialise(int _width, int _height);
	void Update();
	void Draw();

	static Game& GetGameInstance();
	static void DestroyGameInstance();

	void SetScreenMouseX(int _x);
	void SetScreenMouseY(int _y);


	InputManager* GetInputManager();

protected:
	//Singleton
	static Game* currentGame;

private:
	ShaderLoader * shaderLoader;
	TextureLoader* textureLoader;
	InputManager* inputManager;
	Level* currentLevel;
	std::vector<Level*> vecLevels;

	int currentRows;
	int currentColumns;
	int screenMouseX;
	int screenMouseY;
};