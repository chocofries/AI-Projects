#include "pathfinder.h"

Pathfinder::Pathfinder()
{
	isFinding = false;
	isResizing = false;
}

Pathfinder::~Pathfinder()
{
}

void Pathfinder::SetRows(unsigned int _rows)
{
	rows = _rows;
}

void Pathfinder::SetColumns(unsigned int _columns)
{
	columns = _columns;
}

void Pathfinder::SetTileGrid(int row, int col, Tile * tile)
{
	tileGrid[row][col] = tile;
}

void Pathfinder::SetVecTiles(std::vector<GameObject*>* _vecTiles)
{
	vecTiles = _vecTiles;
}

void Pathfinder::SetStartTile(Tile * start)
{
	startTile = start;
}

void Pathfinder::SetDestTile(Tile * dest)
{
	destTile = dest;
}

Tile * Pathfinder::GetStartTile()
{
	return startTile;
}

Tile * Pathfinder::GetDestTile()
{
	return destTile;
}

bool Pathfinder::GetIsFinding() const
{
	return isFinding;
}

bool Pathfinder::GetIsResizing() const
{
	return isResizing;
}

int Pathfinder::GetRows() const
{
	return rows;
}

int Pathfinder::GetColumns() const
{
	return columns;
}

void Pathfinder::Starfinder()
{
	isFinding = true;
	//Ensure that starttile and desttile is both set
	if (startTile == nullptr || destTile == nullptr)
	{
		std::cout << "ERROR: startTile or destTile not set properly." << std::endl;
		isFinding = false;
		return;
	}

	//Ensure openlist and closedlist is empty
	openList.clear();
	closedList.clear();

	//Put starttile in openlist
	openList.push_back(startTile);
	startTile->SetScoreG(0);
	startTile->SetScoreF(HeuristicCost(startTile,destTile)*10);

	int iterations = 0;

	//While openlist is not empty
	while (!openList.empty())
	{
		iterations++;
		if (iterations >= GRID_MAX_ROWS*GRID_MAX_COLS)
		{
			std::cout << "Path is blocked!" << std::endl;
			isFinding = false;
			return;
		}

		//Take lowest fvalue tile from openlist, set as currenttile
		std::vector<Tile*>::iterator it;
		int lowestF = 10000;
		for (it = openList.begin(); it != openList.end(); it++)
		{
			Tile* tempTile = *it;
			if (tempTile->GetScoreF() < lowestF)
			{
				lowestF = tempTile->GetScoreF();
				currentTile = tempTile;
			}
		}

		//If the currenttile is the desttile, then the solution is found
		if (currentTile->GetTileState() == TILE_DEST)
		{
			//Generate a path from here back to the starttile using the parent pointers
			while(currentTile->GetTileState() != TILE_START)
			{
				if (currentTile->GetTileState() != TILE_DEST)
				{
					currentTile->SetTileState(TILE_PATH);
				}

				Tile* parent = currentTile->GetParentTile();

				if (parent != nullptr)
				{
					currentTile = parent;
				}

				if (iterations >= GRID_MAX_ROWS * GRID_MAX_COLS)
				{
					std::cout << "Path is blocked!" << std::endl;
					isFinding = false;
					return;
				}
			}

			std::cout << "Path Found!" << std::endl;
			isFinding = false;
			return;
		}

		//Remove the current from the openlist and add to closedlist
		openList.erase(std::remove(openList.begin(),openList.end(),currentTile),openList.end());
		closedList.push_back(currentTile);

		if (currentTile->GetTileState() != TILE_START && currentTile->GetTileState() != TILE_DEST)
		{
			currentTile->SetTileState(TILE_CLOSED);
		}

		int cX = currentTile->GetGridX();
		int cY = currentTile->GetGridY();

		//Generate a list of neighbourtiles from the currenttile
		std::vector<Tile*> neighbours;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) //Skip if its the currenttile
				{
					continue;
				}

				int sX = cX + i;
				int sY = cY + j;

				if (sX >= 0 && sY >= 0 && sX < (int)columns && sY < (int)rows) //If the neighbour is in-bounds of grid
				{
					Tile* neighbour = tileGrid[sX][sY];
					TileState nState = neighbour->GetTileState();
					if (nState != TILE_OBS || nState != TILE_CLOSED || nState != TILE_START) //If neighbour is able to be checked
					{
						neighbours.push_back(neighbour);
					}
				}
			}
		}

		for (unsigned int n = 0; n < neighbours.size(); n++)
		{
			Tile* nt = neighbours[n];

			if (nt->GetTileState() == TILE_CLOSED || nt->GetTileState() == TILE_START || nt->GetTileState() == TILE_OBS)
			{
				continue;
			}

			int tentativeScore = (FindDistance(currentTile, nt)); //The G Score

			if (nt->GetTileState() != TILE_OPEN) //If neighbour is not in the openlist
			{
				if (nt->GetTileState() != TILE_DEST)
				{
					nt->SetTileState(TILE_OPEN);
				}
				openList.push_back(nt);
			}
			else if (tentativeScore >= nt->GetScoreG())
			{
				continue; //This is not a better path
			}

			//This path is the best so far
			nt->SetScoreH(HeuristicCost(nt,destTile));
			nt->SetParentTile(currentTile);
			nt->SetScoreG(tentativeScore);
			nt->SetScoreF(nt->GetScoreG() + nt->GetScoreH());
		}

		currentTile->Update();
	}
	std::cout << "Path is blocked!" << std::endl;
	isFinding = false;
}

void Pathfinder::Reset(bool resetObstacles)
{
	for (int i = 0; i < GRID_MAX_ROWS; i++)
	{
		for (int j = 0; j < GRID_MAX_COLS; j++)
		{
			Tile* temp = tileGrid[i][j];

			if (temp != nullptr)
			{
				TileState tileState = temp->GetTileState();

				temp->SetScoreF(0);
				temp->SetScoreG(0);
				temp->SetScoreH(0);

				if (resetObstacles)
				{
					if (tileState != TILE_START && tileState != TILE_DEST)
					{
						temp->SetTileState(TILE_FREE);
					}
				}
				else
				{
					if (tileState != TILE_START && tileState != TILE_DEST && tileState != TILE_OBS)
					{
						temp->SetTileState(TILE_FREE);
					}
				}
			}
		}
	}
}

void Pathfinder::ResizeGrid(int newRows, int newCols)
{
	isResizing = true;
	if (!isFinding)
	{
		if (newRows <= GRID_MAX_ROWS && newCols <= GRID_MAX_COLS)
		{
			int prevRows = rows;
			int prevCols = columns;
			rows = newRows;
			columns = newCols;

			float halfRow = (float)(rows - 1) * 0.5f;
			float halfColumn = (float)(columns - 1) * 0.5f;
			float tileScale = (float)((WINDOW_WIDTH + 100) / columns);
			float gap = 1.0f;
			float offset = (tileScale*0.5f) + gap;

			startTile->SetTileState(TILE_FREE);
			destTile->SetTileState(TILE_FREE);

			for (int i = 0; i < GRID_MAX_ROWS; i++)
			{
				for (int j = 0; j < GRID_MAX_COLS; j++)
				{
					Tile* temp = tileGrid[i][j];

					if (temp != nullptr)
					{
						if (i >= newRows || j >= newCols) //Out of bounds
						{
							temp->SetX((offset*(float)i) - (halfColumn * offset));
							temp->SetY((offset*-(float)j) + (halfRow * offset));
							temp->SetXScale(tileScale);
							temp->SetYScale(tileScale);
							temp->SetTextActive(false);
							temp->SetIsActive(false);
						}
						else //Within bounds
						{
							if (i == 0 && j == 0)
							{
								temp->SetTileState(TILE_START);
								startTile = temp;
							}
							if (i == newRows-1 && j == newCols-1)
							{
								temp->SetTileState(TILE_DEST);
								destTile = temp;
							}
							temp->SetX((offset*(float)i) - (halfColumn * offset));
							temp->SetY((offset*-(float)j) + (halfRow * offset));
							temp->SetXScale(tileScale);
							temp->SetYScale(tileScale);
							temp->SetIsActive(true);
						}
					}
				}
			}
		}
	}
	isResizing = false;
}

int Pathfinder::HeuristicCost(Tile * first, Tile * second)
{
	int hCost = 0;
	int firstX = first->GetGridX();
	int firstY = first->GetGridY();
	int secondX = second->GetGridX();
	int secondY = second->GetGridY();

	hCost = (abs(secondX - firstX) + abs(secondY - firstY)) * 10;

	return hCost;
}

int Pathfinder::FindDistance(Tile * current, Tile * neighbour)
{
	int distance = 0;
	int cost = 0;
	int cX = current->GetGridX();
	int cY = current->GetGridY();
	int nX = neighbour->GetGridX();
	int nY = neighbour->GetGridY();

	int cTotal = cX + cY;
	int nTotal = nX + nY;

	if (cTotal % 2 == 0 && nTotal % 2 == 0) //If cTotal is even and nTotal is even
	{
		cost = 14; //It is diagonal
	}
	else if (cTotal % 2 != 0 && nTotal % 2 != 0) //If cTotal is odd and nTotal is odd
	{
		cost = 14; //It is diagonal
	}
	else //If cTotal and nTotal are different
	{
		cost = 10; //It is orthogonal
	}
	
	distance = current->GetScoreG() + cost;

	return distance;
}
