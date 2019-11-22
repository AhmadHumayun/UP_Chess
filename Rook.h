#pragma once
#include "Piece.h"
class Rook : public Piece
{
public:
	Rook(int block, int c, const char* bmp);
	bool RecalculateMS();
	bool stopMoving(int id, int &counter, bool &kingCheck);
};
