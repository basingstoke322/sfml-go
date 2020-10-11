class computerPlayer : public player
{
private:
public:
	computerPlayer(int c):player(c)
	{
	}
	std::vector<move> getMoves(board *Board, int color)
	{
		std::vector<move> res;
		int oppositeColor = getOppositeColor(color);
		for(int i = 0; i < Board->getSize(); i++)
		{
			for(int j = 0; j < Board->getSize(); j++)
			{
				if(Board->isFree(i,j)) 
				{
					std::vector <cell*> neighbours = Board->getNeighboursForMove(i, j);
					int mas[4] = {};
					for(int k = 0; k < neighbours.size(); k++)
					{
						int m = neighbours[k]->getColor();
						if (!(m < 0))
							++mas[m];
					}
					neighbours.clear();
					if ((mas[oppositeColor]!=3)&&(mas[color] > 0))
					{
						move m;
						m.i = i;
						m.j = j;
						m.score = 0;
						res.push_back(m);						
					}
				}
			}
		}

		int count = (int)(getBoardFreedom(Board) % 10);
		int x, y;

		for(int i = 0; i < count; i++)
		{
			for(int j = 0; j < count; j++)
			{
				move m;
				do
				{
					x = getRandomNumber(0, Board->getSize());
					y = getRandomNumber(0, Board->getSize());
				} while(!(Board->isFree(x,y)));
				m.i = x;
				m.j = y;
				m.score = 0;
				res.push_back(m);
			}
		}
		return res;
	}
	std::vector<move> getMovesFull(board *Board, int color)
	{
		std::vector<move> res;
		int oppositeColor = getOppositeColor(color);
		for(int i = 0; i < Board->getSize(); i++)
		{
			for(int j = 0; j < Board->getSize(); j++)
			{
				if(Board->isFree(i,j)) 
				{
					move m;
					m.i = i;
					m.j = j;
					m.score = 0;
					res.push_back(m);						
				}
			}
		}
		return res;
	}
	move deepMoveSearch(int depth, int color, int startColor, int alpha, int beta, board *Board)
	{
		move Move;
		std::vector <move> moves = getBoardFreedom(Board) < 25 ? getMoves(Board, color) : getMoves(Board, color);
		int m = color == startColor ? -32000 : 32000; 
		int oppositeColor = getOppositeColor(color);

		for(int k = 0; k < (int)(moves.size()); k++)
		{
			board testBoard(Board->getSize());
			for(int i = 0; i < testBoard.getSize(); i++)
			{
				for(int j = 0; j < testBoard.getSize(); j++)
				{
					testBoard.getCell(i, j)->setColor(Board->getCell(i, j)->getColor());
				}
			}
			makeMove(moves[k].i, moves[k].j, &testBoard);

			if (depth > 1) 
			{
				moves[k].score = deepMoveSearch(depth - 1, oppositeColor, startColor, alpha, beta, &testBoard).score;
			}
			else
			{
				moves[k].score = calcTerritory(&testBoard, color) - calcTerritory(&testBoard, oppositeColor);
			}

			if(color == startColor)
			{
				if (alpha < moves[k].score)
				{
					alpha = moves[k].score;
				}
				if (beta <= alpha)
				{
					break;
				}
			}
			else
			{
				if (beta > moves[k].score)
				{
					beta = moves[k].score;
				}
				if (beta <= alpha)
				{
					break;
				}
			}
			
		}
		for(int k = 0; k < moves.size(); k++)
		{
			if (color == startColor)
			{
				if (moves[k].score > m)
				{
					m = moves[k].score;
					Move = moves[k];
				}
			}
			else
			{
				if (moves[k].score < m)
				{
					m = moves[k].score;
					Move = moves[k];
				}
			}				
		}
		moves.clear();
		return Move;
	}
	move shortMoveSearch(int color, board *Board)
	{
		move Move;
		Move.score = -1000;
		std::vector <move> moves = getMovesFull(Board, color);
		int oppositeColor = getOppositeColor(color);

		for(int k = 0; k < (int)(moves.size()); k++)
		{
			board testBoard(Board->getSize());
			for(int i = 0; i < testBoard.getSize(); i++)
			{
				for(int j = 0; j < testBoard.getSize(); j++)
				{
					testBoard.getCell(i, j)->setColor(Board->getCell(i, j)->getColor());
				}
			}
			makeMove(moves[k].i, moves[k].j, &testBoard);
			moves[k].score = calcTerritory(&testBoard, color) - calcTerritory(&testBoard, oppositeColor);
			if (Move.score < moves[k].score)
			{
				Move = moves[k];
			}
		}
		return Move;
	}
};