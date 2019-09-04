#include "minimax.h"
#include <algorithm>

Minimax::Minimax()
{
	myGame = nullptr;
}

Minimax::Minimax(Game* _game)
{
	myGame = _game;
	current = new Node();
	current->SetIsMax(false);

	//Copy over current game's grid to minimax
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			current->SetGrid(j, i, myGame->GetAllGrid(j, i));
		}
	}
}

Minimax::~Minimax()
{
	while (vecNodes.size() > 0)
	{
		Node* tempNode = vecNodes[vecNodes.size() - 1];
		vecNodes.pop_back();
		delete tempNode;
	}
	delete current;
}

void Minimax::BestMove()
{
	Node* re = ReMinimax(current, 0, true, myGame->GetLastX(), myGame->GetLastY());
	Node* best = AlphaBeta(re, -1000, 1000);
	
	std::vector<Node*>::iterator it;

	Node* bestNode = nullptr;
	int highestScore = -10;
	
	if (!best->GetChildren().empty())
	{
		bestNode = best->GetChildren().back();
		highestScore = bestNode->GetScore();
	}
	else
	{
		bestNode = best;
	}

	for (it = best->GetChildren().begin(); it != best->GetChildren().end(); it++)
	{
		Node* tempNode = *it;
		if (tempNode->GetScore() > highestScore)
		{
			highestScore = tempNode->GetScore();
			bestNode = tempNode;
		}
	}
	
	bestX = bestNode->GetMoveX();
	bestY = bestNode->GetMoveY();
}

Node* Minimax::ReMinimax(Node * _prevnode, int depth, int _player, int _mX, int _mY)
{
	Node* myNode = new Node();
	myNode->SetPrevNode(_prevnode);
	_prevnode->GetChildren().push_back(myNode);
	myNode->SetDepth(depth);
	vecNodes.push_back(myNode); //Add this node to the minimax tree's node vector

	int myPlayer = 0;

	//If previous node is max, set my node to min, else set to max
	if (_prevnode->GetIsMax())
	{
		myNode->SetIsMax(false);
		myPlayer = 2; //Min player (cross)
	}
	else
	{
		myNode->SetIsMax(true);
		myPlayer = 1; //Max player (circle)
	}

	//Copy over previous nodes's grid to my grid
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			myNode->SetGrid(i, j, _prevnode->GetGrid(i, j));
		}
	}

	//Make the new move on my node's grid
	if (depth > 0)
	{
		myNode->SetGrid(_mX, _mY, myPlayer);
		myNode->SetMoveX(_mX);
		myNode->SetMoveY(_mY);
	}

	//If this move is a winner
	if (myNode->CheckWin(myPlayer))
	{
		return myNode; //Return and break out of the function
	}
	else //If this move is still not a winner
	{
		//Find empty spaces in my node
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (myNode->GetGrid(i, j) == 0) //If the space is empty
				{
					Node* tempNode = ReMinimax(myNode, myNode->GetDepth()+1, myPlayer, i, j); //Recursively create child nodes that play in the empty positions
				}
			}
		}
	}

	return myNode;
}

Node* Minimax::AlphaBeta(Node * _node, int _alpha, int _beta)
{
	std::vector<Node*>::iterator it;

	if (_node->GetChildren().empty() || _node->GetPrevNode() == nullptr) //If it is a leaf node
	{
		return _node;
	}

	_node->SetAlpha(_alpha);
	_node->SetBeta(_beta);

	if (_node->GetIsMax()) //If it is a max node
	{
		_node->SetScore(-1000);
		for (it = _node->GetChildren().begin(); it != _node->GetChildren().end(); it++)
		{
			Node* child = *it;
			Node* parent = _node->GetPrevNode();

			_node->SetScore(std::max(_node->GetScore(), AlphaBeta(child, _node->GetAlpha(), _node->GetBeta())->GetScore()));
			_node->SetAlpha(std::max(_node->GetAlpha(), _node->GetScore()));

			if (_node->GetAlpha() == 10 || _node->GetBeta() == -10) //Prune
			{
				break;
			}
		}
	}
	else //If it is a min node
	{
		_node->SetScore(1000);
		for (it = _node->GetChildren().begin(); it != _node->GetChildren().end(); it++)
		{
			Node* child = *it;
			Node* parent = _node->GetPrevNode();

			_node->SetScore(std::min(_node->GetScore(), AlphaBeta(child, _node->GetAlpha(), _node->GetBeta())->GetScore()));
			_node->SetBeta(std::min(_node->GetBeta(), _node->GetScore()));

			if (_node->GetAlpha() == 10 || _node->GetBeta() == -10) //Prune
			{
				break;
			}
		}
	}

	return _node;
}

int Minimax::GetBestX() const
{
	return bestX;
}

int Minimax::GetBestY() const
{
	return bestY;
}
