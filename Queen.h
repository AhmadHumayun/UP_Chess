#pragma once
#include "Piece.h"
class Queen : public Piece
{
public:
	Queen(int block, int c, const char* bmp);
	bool RecalculateMS();
	bool stopMoving(int id, int &counter, bool &kingCheck);
};
