class player
{
public:
	move previousMove;
	player(int c)
	{
		territory = 0;
		cellCount = 0;
		color = c;
	}
	int calcTerritory(board* Board, int c)
	{
		int x = 0;
		for (int i = 0; i < Board->getSize(); ++i)
		{
			for (int j = 0; j < Board->getSize(); ++j)
			{
				if (Board->getCell(i, j)->getOwner() == c)
					x++;
			}
		}
		return x;
	}
	int calcCells(board* Board, int c)
	{
		int x = 0;
		for (int i = 0; i < Board->getSize(); ++i)
		{
			for (int j = 0; j < Board->getSize(); ++j)
			{
				if (Board->getCell(i, j)->getColor() == c)
					x++;
			}
		}
		return x;
	}
	int getTerritory()
	{
		return territory;
	}
	int getPlayerColor()
	{
		return color;
	}
	void makeMove(int i, int j, board *Board)
	{
		Board->getCell(i,j)->setColor(color);
		Board->clearDead(getOppositeColor(color));
		Board->clearDead(color);
		territory = calcTerritory(Board, color) + (color == 2 ? 6.5 : 0);
		previousMove.i = i;
		previousMove.j = j;
	}
	int getPoints()
	{
		return getTerritory();
	}
	int getBoardFreedom(board * Board)
	{
		return pow(Board->getSize(), 2) - calcCells(Board, color) - calcCells(Board, getOppositeColor(color));
	}
protected:
	float territory = 0;
	int cellCount = 0;
	int color;
};