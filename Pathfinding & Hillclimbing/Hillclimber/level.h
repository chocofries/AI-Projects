#pragma once
#include "game.h"
#include "gameobject.h"
#include "hillclimber.h"

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
	Hillclimber* GetPathfinder();
	bool GetIsInit() const;

	void SetScreenMouseX(int _x);
	void SetScreenMouseY(int _y);

private:
	GLuint levelVBO;
	Game* myGame;
	ShaderLoader* shaderLoader;
	TextureLoader* textureLoader;
	InputManager* inputManager;
	Hillclimber* hillclimber;
	FMOD::System* audioSystem;
	Camera* levelCamera;
	std::vector<GameObject*> vecObjects;
	std::vector<Texter*> vecTexts;

	bool pathFound;
	bool isInit;
	Texter* text1;
	Texter* text2;

	unsigned int rows;
	unsigned int columns;

	int screenMouseX;
	int screenMouseY;
	glm::vec3 rayWorld;
	glm::vec3 intersection;
};