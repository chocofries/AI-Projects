#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include "shaderloader.h"
#include "gametile.h"
#include "minimax.h"

class GameTile;

class Game
{
public:
	Game();
	~Game();

	void Initialise();
	void Draw();
	void PrintText(float _x, float _y, std::string _string);
	void Update();
	void MouseMove(int _x, int _y);
	void MouseClick(int _button, int _state, int _x, int _y);
	void KeyPress(unsigned char _key, int _x, int _y);
	void NextPlayerTurn();
	void AI();
	void GameRestart();
	void Sleep(int _msecs);
	void UpdateAllGrid();

	bool CheckWin(int _player); //0 = empty, 1 = circle, 2 = cross, 3 = draw

	void SetCircleGrid(int _x, int _y, bool _set);
	void SetCrossGrid(int _x, int _y, bool _set);
	void SetLastMove(int _x, int _y);
	void SetMoveInputed(bool _inputed);
	bool GetCircleGrid(int _x, int _y) const;
	bool GetCrossGrid(int _x, int _y) const;
	bool GetIsPlayerTurn() const;
	bool GetIsVersusAI() const;
	bool GetGameState() const;
	bool GetMoveInputed() const;
	int GetAllGrid(int _x, int _y) const;
	int GetTurnNumber() const;
	int GetLastX() const;
	int GetLastY() const;

private:
	int gameDifficulty;
	int turnNumber;
	bool startPlayer;
	bool isPlayerTurn;
	bool isVersusAI;
	bool moveInputed;
	std::vector<GameTile*> vecTiles;
	bool circleGrid[3][3];
	bool crossGrid[3][3];
	int allGrid[3][3]; //0 = empty, 1 = circle, 2 = cross
	int gameState; //0 = no wins, 1 = circle won, 2 = cross won
	GLuint VBO;

	int mouseX;
	int mouseY;
	int lastX; //The last move played X
	int lastY; //The last move played Y

};