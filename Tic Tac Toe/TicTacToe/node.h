#pragma once

#include <iostream>
#include "game.h"
#include "minimax.h"

class Node
{
public:
	Node();
	~Node();
	
	bool CheckWin(int _player);

	bool GetIsMax() const;
	int GetScore() const;
	int GetDepth() const;
	int GetGrid(int _x, int _y) const;
	int GetMoveX() const;
	int GetMoveY() const;
	int GetAlpha() const;
	int GetBeta() const;
	Node* GetPrevNode() const;
	Node* GetBestNode() const;
	std::vector<Node*> &GetChildren();

	void SetGrid(int _x, int _y, int _player);
	void SetIsMax(bool _isMax);
	void SetPrevNode(Node* _prev);
	void SetBestNode(Node* _best);
	void SetScore(int _score);
	void SetDepth(int _depth);
	void SetMoveX(int _x);
	void SetMoveY(int _y);
	void SetAlpha(int _a);
	void SetBeta(int _b);

private:
	std::vector<Node*> children;
	Node* prevNode;
	Node* bestNode;
	bool isMax; //false = min, true = max
	int score;
	int depth;
	int alpha;
	int beta;
	int grid[3][3];
	int moveX; //The x position of this node's move
	int moveY; //The y position of this node's move

};