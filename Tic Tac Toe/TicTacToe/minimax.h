#pragma once

#include <iostream>
#include <vector>
#include "game.h"
#include "node.h"

class Node;
class Game;

class Minimax
{
public:
	Minimax();
	Minimax(Game* _game);
	~Minimax();
	
	void BestMove();
	Node* ReMinimax(Node* _prevnode, int depth, int _player, int _mX, int _mY);
	Node* AlphaBeta(Node* _node, int _alpha, int _beta);

	int GetBestX() const;
	int GetBestY() const;

private:
	int bestX;
	int bestY;
	Game* myGame;
	Node* current;
	std::vector<Node*> vecNodes;

};