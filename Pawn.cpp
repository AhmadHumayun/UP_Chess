#include "Pawn.h"
#include "GameManager.h"

Pawn::Pawn(int block, int c, const char * bmp) : Piece(block, c, bmp)
{
	type = PAWN;
}

bool Pawn::RecalculateMS()
{
	Piece** board = GameManager::GetTracker();

	//setting up offset values
	int FRONT = 8;
	int TWOSTEPS= 16;
	int LEFT = 7;
	int RIGHT = 9;

	bool kingAttack = false;

	//Direction -1 if white, 1 if black
	int direction = -1;

	if (color == BLACK)
		direction = 1;

	//possible moveset values (after adding the offsets)
	FRONT = block + (direction * FRONT);
	TWOSTEPS = block + (direction * TWOSTEPS);
	LEFT = block + (direction * LEFT);
	RIGHT = block + (direction * RIGHT);

	for (int i = 0; i < 64; i++)
		moveset[i] = -1;

	if (board[FRONT] == NULL  && ( FRONT <  64 && FRONT > -1) ) //checks if the first block is empty and within range
	{
		moveset[0] = FRONT;
		
		if (first_move && board[TWOSTEPS] == NULL)  //checks if its the second turn and the block is empty 
		{
			moveset[1] = TWOSTEPS;
		}
	}

	if (board[RIGHT] == NULL)
		goto right;

	if ((RIGHT < 64 && RIGHT > -1) && board[RIGHT]->GetColor() != color) //checks if right diagnol move is within range and has an enemy piece
	{
		moveset[2] = RIGHT;
		if (board[RIGHT]->GetType() == KING)
			kingAttack = true;
	}

right:
	if (board[LEFT] == NULL)
		return kingAttack;

	if ((LEFT < 64 && LEFT > -1) && board[LEFT]->GetColor() != color) //checks if left diagnol move is within range and has an enemy piece
	{
		moveset[3] = LEFT;
		if (board[LEFT]->GetType() == KING)
			kingAttack = true;
	}

	return kingAttack;  //returns true if the pawn can attack the king (i.e king in check)
}
