#pragma once
#include "game.h"
#include "gameobject.h"
#include "pathfinder.h"

class Game;

class Level
{
public:
	Level();
	virtual ~Level();

	virtual void Initialise(Game* _myGame, ShaderLoader* _shaderloader, TextureLoader* _textureLoader, InputManager* _inputManager, int _rows, int _columns);
	virtual void Update();
	virtual void Draw();

	void ProcessKeyInput();

	std::vector<Texter*>* GetVecTexts();
	Pathfinder* GetPathfinder();
	bool GetIsInit() const;

	void SetScreenMouseX(int _x);
	void SetScreenMouseY(int _y);

	void CalculateRaycast();

private:
	GLuint levelVBO;
	Game* myGame;
	ShaderLoader* shaderLoader;
	TextureLoader* textureLoader;
	InputManager* inputManager;
	Pathfinder* pathfinder;
	FMOD::System* audioSystem;
	Camera* levelCamera;
	std::vector<GameObject*> vecObjects;
	std::vector<Texter*> vecTexts;

	bool pathFound;
	bool isInit;

	unsigned int rows;
	unsigned int columns;

	int screenMouseX;
	int screenMouseY;
	glm::vec3 rayWorld;
	glm::vec3 intersection;
};