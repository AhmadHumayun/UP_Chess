#pragma once
#include "Piece.h"
class Knight : public Piece
{
public:
	Knight(int block, int c, const char* bmp);
	bool RecalculateMS();
};
