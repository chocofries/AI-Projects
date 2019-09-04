#include "node.h"

Node::Node()
{
	score = 0;
	alpha = 0;
	beta = 0;
}

Node::~Node()
{
}

bool Node::CheckWin(int _player)
{
	//Horizontal/Verticals
	for (int v = 0; v < 3; v++)
	{
		for (int h = 0; h < 3; h++)
		{
			if ((grid[0][v] == _player && grid[1][v] == _player && grid[2][v] == _player) ||
				(grid[h][0] == _player && grid[h][1] == _player && grid[h][2] == _player))
			{
				if (isMax)
				{
					score = -10; //If max node (but player wins), then set score to -10
				}
				else
				{
					score = 10; //If min (but computer wins), then set score to 10
				}
				return true;
			}
		}
	}

	//Diagonals
	if ((grid[0][0] == _player && grid[1][1] == _player && grid[2][2] == _player) ||
		(grid[0][2] == _player && grid[1][1] == _player && grid[2][0] == _player))
	{
		if (isMax)
		{
			score = -10; //If max node (but player wins), then set score to -10
		}
		else
		{
			score = 10; //If min (but computer wins), then set score to 10
		}
		return true;
	}

	return false;
}

bool Node::GetIsMax() const
{
	return isMax;
}

int Node::GetScore() const
{
	return score;
}

int Node::GetDepth() const
{
	return depth;
}

int Node::GetGrid(int _x, int _y) const
{
	return grid[_x][_y];
}

int Node::GetMoveX() const
{
	return moveX;
}

int Node::GetMoveY() const
{
	return moveY;
}

int Node::GetAlpha() const
{
	return alpha;
}

int Node::GetBeta() const
{
	return beta;
}

Node * Node::GetPrevNode() const
{
	return prevNode;
}

Node * Node::GetBestNode() const
{
	return bestNode;
}

std::vector<Node*> &Node::GetChildren()
{
	return children;
}

void Node::SetGrid(int _x, int _y, int _player)
{
	grid[_x][_y] = _player;
}

void Node::SetIsMax(bool _isMax)
{
	isMax = _isMax;
}

void Node::SetPrevNode(Node * _prev)
{
	prevNode = _prev;
}

void Node::SetBestNode(Node * _best)
{
	bestNode = _best;
}

void Node::SetScore(int _score)
{
	score = _score;
}

void Node::SetDepth(int _depth)
{
	depth = _depth;
}

void Node::SetMoveX(int _x)
{
	moveX = _x;
}

void Node::SetMoveY(int _y)
{
	moveY = _y;
}

void Node::SetAlpha(int _a)
{
	alpha = _a;
}

void Node::SetBeta(int _b)
{
	beta = _b;
}
