#include "Utils.h"
#include "Constants.h"

Coord Utils::BlockToIndex(int b)
{
	Coord index;

	int y = b / 8;
	int x = b - 8 * y;

	index.x = x;
	index.y = y;

	return index;
}

Coord Utils::BlockToScreen(int b)
{
	Coord render_pos = BlockToIndex(b);

	render_pos.x = render_pos.x * con::BLOCK_SIZE;
	render_pos.y = render_pos.y * con::BLOCK_SIZE;

	return render_pos;
}

int Utils::IndexToBlock(int x, int y)
{
	return x + 8 * y;
}
