#include "Renderer.h"
#include <windows.h>
using namespace std;

int Renderer::marker;
int Renderer::selected;
bool Renderer::select_active;

//Helpful function to remove reperitive code
COLORREF COLOR(unsigned int hex)
{
	//Separate RGB values and return COLORREF
	unsigned char R = hex >> 16 & 0xff;
	unsigned char G = hex >> 8 & 0xff;
	unsigned char B = hex & 0xff;

	return RGB(R, G, B);
}

void Renderer::Draw(Pixel** b, int x, int y)
{
	HWND console = GetConsoleWindow();
	HDC cdc = GetDC(console);
	
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			//Absolute delta of rgb values relative to background color
			int del_R = abs(b[i][j].R - BG_COLOR_R);
			int del_G = abs(b[i][j].G - BG_COLOR_G);
			int del_B = abs(b[i][j].B - BG_COLOR_B);
			
			//if the color is in the pinkish range drop pixel
			if (del_R + del_G + del_B < 100)
				continue;
			
			//Else render the pixel
			SetPixel(cdc, x + i + SCREEN_OFFSET_X, y + j + SCREEN_OFFSET_Y, RGB(b[i][j].R, b[i][j].G, b[i][j].B));
		}
	}
}

//Helpful function.. returns 1 or 0 to act as an index to a size 2 array in constants.h
int BlockColorIndex(int block)
{
	Coord index_xy = Utils::BlockToIndex(block);
	int r = index_xy.x;
	int c = index_xy.y;

	return (r % 2) ^ (c % 2);
}

void Renderer::DrawBlock(int block)
{
	HWND console = GetConsoleWindow();
	HDC cdc = GetDC(console);
	Coord screen_xy = Utils::BlockToScreen(block);

	int x = screen_xy.x;
	int y = screen_xy.y;

	//If both indices even then square is black else white
	int color = BlockColorIndex(block);

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			//Set background pixel color
			SetPixel(cdc, x + i + SCREEN_OFFSET_X, y + j + SCREEN_OFFSET_Y, COLOR(BLOCK_COLOR[color]));
		}
	}

	//If the user has marked the block draw a yellow marker
	if (block == marker)
		MarkBlock(block, MARKER_COLOR);

	//If user has selected the block draw a red marker
	if (block == selected && select_active)
		MarkBlock(block, SELECTED_MARKER_COLOR);

	ReleaseDC(console, cdc);
}

void Renderer::DrawBoard()
{
	for (int i = 0; i < 64; i++)
		DrawBlock(i);
}

void Renderer::EraseMarker(int block)
{
	int color = BLOCK_COLOR[BlockColorIndex(block)];
	MarkBlock(block, color);
}

void Renderer::MarkBlock(int block, int color)
{
	Coord pos = Utils::BlockToScreen(block);
	int x = pos.x;
	int y = pos.y;
	HWND console = GetConsoleWindow();
	HDC cdc = GetDC(console);

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			//Only colors uptil 5 pixels from the edges 
			if (j < MARKER_WIDTH || j > BLOCK_SIZE - MARKER_WIDTH || i < MARKER_WIDTH || i > BLOCK_SIZE - MARKER_WIDTH)
				SetPixel(cdc, x + i + SCREEN_OFFSET_X, y + j + SCREEN_OFFSET_Y, COLOR(color));
		}
	}

	ReleaseDC(console, cdc);
}

void Renderer::ReadImage(const char * path, Pixel** &rgbData)
{
	ifstream bitmap(path, ios::binary);
	int header_sz = 54;
	int c = header_sz;
	int buffer_sz = 3 * BLOCK_SIZE * BLOCK_SIZE + header_sz;
	char* buffer = new char[buffer_sz];


	//Check if stream is initialized
	if (bitmap.fail())
	{
		cout << "Failed to load stream!!" << endl;
		return;
	}

	//Fill the buffer with pixel data
	bitmap.read(buffer, buffer_sz);
	
	for (int j = BLOCK_SIZE - 1; j >= 0; j--)
	{
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			rgbData[i][j].R = buffer[c + 2];
			rgbData[i][j].G = buffer[c + 1];
			rgbData[i][j].B = buffer[c];
			c += 3;
		}
	}

	delete[] buffer;
}

void Renderer::ReDrawBlocks(int b1, int b2, Pixel** b1_p, Pixel** b2_p)
{
	Coord b1_c = Utils::BlockToScreen(b1);
	Coord b2_c = Utils::BlockToScreen(b2);

	DrawBlock(b1);
	DrawBlock(b2);
	
	if (b1_p)
		Draw(b1_p, b1_c.x, b1_c.y);
	if (b2_p)
		Draw(b2_p, b2_c.x, b2_c.y);
}

int BoundsCorrection(int marker, int dx, int dy)
{
	Coord marker_xy = Utils::BlockToIndex(marker);
	int x = marker_xy.x;
	int y = marker_xy.y;
	int nx = x + dx;
	int ny = y + dy;

	if (nx > 7)
		return Utils::IndexToBlock(0, y);
	else if (nx < 0)
		return Utils::IndexToBlock(7, y);
	else if (ny > 7)
		return Utils::IndexToBlock(x, 0);
	else if (ny < 0)
		return Utils::IndexToBlock(x, 7);
	else
		return marker + dx + dy * 8;
}

void Renderer::UpdateMarker(char input)
{
	int delta_x = 0;
	int delta_y = 0;

	switch (input)
	{
	case 'w':
		delta_y = -1;
		break;
	case 'a':
		delta_x = -1;
		break;
	case 's':
		delta_y = 1;
		break;
	case 'd':
		delta_x = 1;
		break;
	}

	if(marker != selected)
		EraseMarker(marker);

	marker = BoundsCorrection(marker, delta_x, delta_y);

	if(marker != selected)
		MarkBlock(marker, MARKER_COLOR);
}

void Renderer::SetMarker(int v)
{
	marker = v;
}

int Renderer::GetMarker()
{
	return marker;
}

void Renderer::SetSelected(int v)
{
	selected = v;
}

void Renderer::SetSelect_Active(bool v)
{
	select_active = v;
}
