/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	level.h
Description	:	Level class for OpenGl game engine functionality
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/

#pragma once
#include "game.h"
#include "gameobject.h"
#include "clock.h"

class Game;
class Robot;
class Bomb;
class Genetic;

class Level
{
public:
	Level();
	virtual ~Level();

	virtual void Initialise(Game* _myGame, ShaderLoader* _shaderloader, TextureLoader* _textureLoader, InputManager* _inputManager);
	virtual void Update();
	virtual void Draw();

	void ProcessKeyInput();

	std::vector<Texter*>* GetVecTexts();
	std::vector<Robot*>* GetVecRobots();
	std::vector<Bomb*>* GetVecBombs();
	GLuint GetVBO() const;
	ShaderLoader* GetShaderLoader() const;
	TextureLoader* GetTextureLoader() const;
	InputManager* GetInputManager() const;
	Camera* GetCamera() const;
	Clock* GetClock() const;

	bool GetIsInit() const;

	void SetScreenMouseX(int _x);
	void SetScreenMouseY(int _y);

private:
	GLuint levelVBO;
	Game* myGame;
	ShaderLoader* shaderLoader;
	TextureLoader* textureLoader;
	InputManager* inputManager;
	FMOD::System* audioSystem;
	Camera* levelCamera;
	Clock* clock;
	Genetic* genetic;
	std::vector<GameObject*> vecObjects;
	std::vector<Texter*> vecTexts;
	std::vector<Robot*> vecRobots;
	std::vector<Bomb*> vecBombs;

	Texter* textA;

	bool isInit;

	int badPercentage; //Percentage of bad robots
	int screenMouseX;
	int screenMouseY;
	glm::vec3 rayWorld;
	glm::vec3 intersection;
};