#pragma once
#include "Renderer.h"

class Piece
{
protected:
	Coord board_pos;
	Coord screen_pos;
	Pixel** pixels;
	int block;
	int color;
	int type;
	int moveset[64];
	bool active = true;
	bool first_move = true;

public:

	Piece(int b, int c, const char* bmp);
	~Piece();

	/*Recalculate Moveset. Returns true
	if opposing team's king is in check*/
	virtual bool RecalculateMS() = 0;

	virtual bool CanMove(int block);

	void Move(int block, bool caller = false);

	Coord GetBoardPos();
	Coord GetScreenPos();
	int GetBlock();
	int GetColor();
	int GetType();
	int* GetMoveSet();
	bool GetAcive();
	Pixel** GetPixels();

	void SetBoardPos(int x, int y);
	void SetScreenPos(int x, int y);
	void SetBlock(int v);
	void SetAcive(bool v);

};