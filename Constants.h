#pragma once
namespace con
{
	const int BLACK = 0;
	const int WHITE = 1;
	const int KING = 2;
	const int QUEEN = 3;
	const int BISHOP = 4;
	const int ROOK = 5;
	const int KNIGHT = 6;
	const int PAWN = 7;

	const int QUIT = 8;
	const int BLACK_WIN = 9;
	const int WHITE_WIN = 10;
	const int STALEMATE = 11;
	const int UNOCCUPIED = 12;

	const int BLOCK_SIZE = 64;
	const int SCREEN_OFFSET_X = 300;
	const int SCREEN_OFFSET_Y = 100;

	const unsigned int MARKER_WIDTH = 5;
	const unsigned int MARKER_COLOR = 0xffff00;
	const unsigned int SELECTED_MARKER_COLOR = 0xff3333;
	const unsigned int BLOCK_COLOR[] = { 0x666999, 0xe6e6e6};
	const unsigned char BG_COLOR_R = 0xfe;
	const unsigned char BG_COLOR_G = 0xae;
	const unsigned char BG_COLOR_B = 0xc9;

	const char BMP_BP[15] = "bmp\\bp.bmp";
	const char BMP_BR[15] = "bmp\\br.bmp";
	const char BMP_BH[15] = "bmp\\bh.bmp";
	const char BMP_BB[15] = "bmp\\bb.bmp";
	const char BMP_BQ[15] = "bmp\\bq.bmp";
	const char BMP_BK[15] = "bmp\\bk.bmp";

	const char BMP_WP[15] = "bmp\\wp.bmp";
	const char BMP_WR[15] = "bmp\\wr.bmp";
	const char BMP_WH[15] = "bmp\\wh.bmp";
	const char BMP_WB[15] = "bmp\\wb.bmp";
	const char BMP_WQ[15] = "bmp\\wq.bmp";
	const char BMP_WK[15] = "bmp\\wk.bmp";
}