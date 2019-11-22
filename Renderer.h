#pragma once
#include "Constants.h"
#include "Utils.h"
using namespace con;

class Renderer
{
private:
	static int marker;
	static int selected;
	static bool select_active;
public:
	static void EraseMarker(int block);
	static void MarkBlock(int block , int color);
	static void Draw(Pixel** b, int x, int y);
	static void DrawBlock(int block);
	static void DrawBoard();
	static void ReadImage(const char* path, Pixel** &rgbData);
	static void ReDrawBlocks(int b1, int b2, Pixel** b1_p = 0, Pixel** b2_p = 0);
	static void UpdateMarker(char v);
	static void SetMarker(int v);

	static int GetMarker();
	static void SetSelected(int v);
	static void SetSelect_Active(bool v);
};