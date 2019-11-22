#pragma once
#include "Renderer.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"
#include "Constants.h"
using namespace con;

class GameManager
{
private:
	static Piece* tracker[64];
	static Piece* pieces_black[16];
	static Piece* pieces_white[16];
	static Piece* threats[3];
	static int result;
	static int turn;
	static bool check;
	static bool selected;
	static bool checkmate;
	static bool quit;
	static int selected_block;
	static int threat_count;
	static int LOS[7];

public:

	//Initializes board, pieces, positions
	static void Init();

	/*Main gameloop, calls update, sets private 
	member "result", returns when match ends*/
	static void StartMatch();

	static void EndGame();
	//Displays result
	static void DisplayResult();
	static Piece** GetTracker();

private:
	//Updates positions
	static void UpdateTracker();
	static void HandleInput();
	static void RefreshScreen();
	static void AttemptMove();
	static void DeselectBlock();
	static void SelectBlock();
	static void Compute_LOS(int attacker_pos, int king_pos);
	static bool CheckThreats(bool color);
	static bool LOS_Breakable();
	static bool Checkmate();
};