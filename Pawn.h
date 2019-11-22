#pragma once
#include "Piece.h"
class Pawn : public Piece
{
public:
	Pawn(int block, int c, const char* bmp);
	bool RecalculateMS();
};
