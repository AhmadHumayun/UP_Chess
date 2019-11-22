#include "Piece.h"

Piece::Piece(int b, int c, const char * bmp)
{
	block = b;
	color = c;
	active = true;
	board_pos = Utils::BlockToIndex(b);
	screen_pos = Utils::BlockToScreen(b);

	//Initialize pixels array on the heap
	pixels = new Pixel*[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++)
		pixels[i] = new Pixel[BLOCK_SIZE];

	//Load the image into the pixels array
	Renderer::ReadImage(bmp, pixels);
}


Piece::~Piece()
{
	for (int i = 0; i < BLOCK_SIZE; i++)
		delete[] pixels[i];

	delete[] pixels;

	pixels = NULL;
}


bool Piece::CanMove(int block)
{
	for (int i = 0; i < 64; i++)
	{
		if (moveset[i] == block)
			return true;
	}

	return false;
}

void Piece::Move(int block, bool caller)
{
	if(!caller)
		first_move = false;


	this->block = block;
	board_pos = Utils::BlockToIndex(block);
	screen_pos = Utils::BlockToScreen(block);
	std::cout << type << " now at block " << block << std::endl;
}


Coord Piece::GetBoardPos()
{
	return board_pos;
}

Coord Piece::GetScreenPos()
{
	return screen_pos;
}

int Piece::GetBlock()
{
	return block;
}

int Piece::GetColor()
{
	return color;
}

int Piece::GetType()
{
	return type;
}

int * Piece::GetMoveSet()
{
	return moveset;
}

bool Piece::GetAcive()
{
	return active;
}

Pixel ** Piece::GetPixels()
{
	return pixels;
}

void Piece::SetBoardPos(int x, int y)
{
	board_pos.x = x;
	board_pos.y = y;
}

void Piece::SetScreenPos(int x, int y)
{
	screen_pos.x = x;
	screen_pos.y = y;
}

void Piece::SetBlock(int v)
{
	block = v;
}


void Piece::SetAcive(bool v)
{
	active = v;
}
