#include "Knight.h"
#include "GameManager.h"

Knight::Knight(int block, int c, const char * bmp) : Piece(block, c, bmp)
{
	type = KNIGHT;
}

bool Knight::RecalculateMS()
{
	int possibleOffsets[4] = { 6, 10, 15, 17 }; //possible offests (not considering the opposite direction)

	bool kingCheck = false;

	Piece** board = GameManager::GetTracker();

	for (int i = 0; i < 64; i++)
		moveset[i] = -1;

	for (int i = 0; i < 8; i++)
	{
		int direction = 1;
		
		if (i % 2)   //considering both directions
			direction = -1;

		int tempMove = block + (direction * possibleOffsets[i / 2]);
		
		if (board[tempMove] == NULL)
		{
			moveset[i] = tempMove;
			continue;
		}
		
		if ((tempMove < 63 && tempMove > -1) && board[tempMove]->GetColor() != color)
		{
			moveset[i] = tempMove;
			if (board[tempMove]->GetType() == KING)
				kingCheck = true;
		}
	}

	return kingCheck;
}
