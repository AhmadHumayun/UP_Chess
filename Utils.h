#pragma once
#include "Structures.h"
#include <iostream>
#include <fstream>

class Utils
{
public:
	static Coord BlockToIndex(int b);
	static Coord BlockToScreen(int b);
	static int IndexToBlock(int x, int y);
};
