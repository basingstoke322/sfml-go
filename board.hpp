class cell
{
public:
	cell(int c = 0)
	{
		color = c;
	}
	int getColor()
	{
		return color;
	}
	int getOwner()
	{
		return owner;
	}
	void setColor(int c)
	{
		color = c;
	}
	void setOwner(int o)
	{
		owner = o;
	}
	int i;
	int j;
	bool checked = true;
	bool alive = false;
	bool checking = false;
private:
	int color;
	int owner;
};

class board
{
public:
	board(int boardSize)
	{
		gameBoard = new cell*[boardSize];
		for(int i = 0; i < boardSize; i++)
		{
			gameBoard[i] = new cell[boardSize];
			for(int j = 0; j < boardSize; j++)
			{
				gameBoard[i][j].i = i;
				gameBoard[i][j].j = j;
			}
		}
		size = boardSize;
	}
	~board()
	{
		for (int i = 0; i < size; i++)
		{
			delete gameBoard[i];
		}
		delete[] gameBoard;
	}
	cell* getCell(int i, int j)
	{
		return &gameBoard[i][j];
	}
	cell* getNeighbour(int i, int j)
	{
		if ((i < size) && (j < size) && (i >= 0) && (j >= 0))
		{
			return getCell(i, j);
		}
		else
		{
			return neighbour;
		}
	}
	bool isFree(int i, int j)
	{
		return (i < size) && (j < size) && (i >= 0) && (j >= 0)&& (gameBoard[i][j].getColor() == 0);
	}
	std::vector <cell*> getNeighbours(int i, int j)
	{
		std::vector<cell*> res;
		res.reserve(4);
		cell * n = getNeighbour(i - 1, j);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i, j-1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i + 1, j);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i, j+1);
		if (n->getColor()!=-1)
			res.push_back(n);
		return res;
	}

	std::vector <cell*> getNeighboursForMove(int i, int j)
	{
		std::vector<cell*> res;
		res.reserve(4);
		cell * n = getNeighbour(i - 1, j);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i, j - 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i + 1, j);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i, j + 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i - 1, j - 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i + 1, j + 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i - 1, j + 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		n = getNeighbour(i + 1, j - 1);
		if (n->getColor()!=-1)
			res.push_back(n);
		return res;
	}

	bool checkLife(cell *current)
	{
		bool l = false;
		std::vector <cell*> neighbours = getNeighbours(current->i, current->j);
		for(int i = 0; i < neighbours.size(); i++)
		{
			if ((!neighbours[i]->getColor())||(neighbours[i]->alive))
			{
				l = true;
				current->checked = true;
				current->alive = true;
			}
		}
		return l;
	}

	void clearCheck()
	{
		for(int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				cell* current = &gameBoard[i][j];
				current->checked = false;
				current->alive = false;
			}
		}
	}
	void clearOwner()
	{
		for(int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				cell* current = &gameBoard[i][j];
				current->setOwner(0);
			}
		}
	}

	void checkOwner()
	{
		clearOwner();
		int color;
		for(int k = 0; k <= size; k++)
		{
			for(int i = 0; i<size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					cell* current = &gameBoard[i][j];
					color = current->getColor();
					if (color > 0)
					{
						current->setOwner(color);
					}
					else if (current->getOwner() == 3)
						continue;
					else
					{
						bool f = false;
						std::vector <cell*> neighbours = getNeighbours(current->i, current->j);
						int mas[4] = {};
						for(int i = 0; i < neighbours.size(); i++)
						{
							++mas[neighbours[i]->getOwner()];
						}
						if((mas[3] > 0)||((mas[1])&&(mas[2])))
						{
							current->setOwner(3);
						}
						else if (mas[1]>0)
							current->setOwner(1);
						else
							current->setOwner(2);
					}
				}
			}
		}
	}

	void clearDead(int color)
	{
		clearCheck();
		for(int k = 0; k <= size + 3; k++)
		{
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					cell* current = &gameBoard[i][j];
					if (!current->checked)
					{
						if (current->getColor() == 0)
						{
							current->alive = true;
						}
						else if (current->getColor() == color)
						{
							current->alive = checkLife(current);
						}					
					}
				}
			}
		}
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				cell * current = &gameBoard[i][j];
				if ((!current->alive)&&(current->getColor() == color))
				{
					current->setColor(0);
				}
			}
		}
		checkOwner();
	}
	int getSize()
	{
		return size;
	}
private:
	cell **gameBoard;
	int size;
	cell * neighbour = new cell(-1);
};