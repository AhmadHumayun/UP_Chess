#include "GameManager.h"
#include "conio.h"
#include <iostream>

Piece* GameManager::tracker[64];
Piece* GameManager::pieces_black[16];
Piece* GameManager::pieces_white[16];
Piece* GameManager::threats[3];
int GameManager::result;
int GameManager::turn;
bool GameManager::check;
bool GameManager::selected;
bool GameManager::quit;
bool GameManager::checkmate;
int GameManager::selected_block;
int GameManager::threat_count;
int GameManager::LOS[7];

void GameManager::RefreshScreen()
{
	//Draw the board first
	Renderer::DrawBoard();

	//Draw Pieces
	for (int i = 0; i < 16; i++)
	{
		// Draw(pixels_piece, x_position, y_position)
		if(pieces_black[i]->GetAcive())
			Renderer::Draw(pieces_black[i]->GetPixels(), pieces_black[i]->GetScreenPos().x, pieces_black[i]->GetScreenPos().y);
	
		if (pieces_white[i]->GetAcive())
			Renderer::Draw(pieces_white[i]->GetPixels(), pieces_white[i]->GetScreenPos().x, pieces_white[i]->GetScreenPos().y);
	}
}

void ClearInputStream()
{
	//Remove all characters from the std in stream
	while (_getch()) {}
}

void GameManager::Init()
{
	turn = WHITE;
	check = false;
	checkmate = false;
	selected = false;
	quit = false;
	threat_count = 0;
	Renderer::SetSelect_Active(false);

	//Initialize black pieces
	pieces_black[0] = new King(3, BLACK, BMP_BK);
	pieces_black[1] = new Queen(4, BLACK, BMP_BQ);
	pieces_black[2] = new Bishop(2, BLACK, BMP_BB);
	pieces_black[3] = new Bishop(5, BLACK, BMP_BB);
	pieces_black[4] = new Knight(1, BLACK, BMP_BH);
	pieces_black[5] = new Knight(6, BLACK, BMP_BH);
	pieces_black[6] = new Rook(0, BLACK, BMP_BR);
	pieces_black[7] = new Rook(7, BLACK, BMP_BR);
	pieces_black[8] = new Pawn(8, BLACK, BMP_BP);
	pieces_black[9] = new Pawn(9, BLACK, BMP_BP);
	pieces_black[10] = new Pawn(10, BLACK, BMP_BP);
	pieces_black[11] = new Pawn(11, BLACK, BMP_BP);
	pieces_black[12] = new Pawn(12, BLACK, BMP_BP);
	pieces_black[13] = new Pawn(13, BLACK, BMP_BP);
	pieces_black[14] = new Pawn(14, BLACK, BMP_BP);
	pieces_black[15] = new Pawn(15, BLACK, BMP_BP);

	//Initialize white pieces
	pieces_white[0] = new King(59, WHITE, BMP_WK);
	pieces_white[1] = new Queen(60, WHITE, BMP_WQ);
	pieces_white[2] = new Bishop(58, WHITE, BMP_WB);
	pieces_white[3] = new Bishop(61, WHITE, BMP_WB);
	pieces_white[4] = new Knight(57, WHITE, BMP_WH);
	pieces_white[5] = new Knight(62, WHITE, BMP_WH);
	pieces_white[6] = new Rook(56, WHITE, BMP_WR);
	pieces_white[7] = new Rook(63, WHITE, BMP_WR);
	pieces_white[8] = new Pawn(48, WHITE, BMP_WP);
	pieces_white[9] = new Pawn(49, WHITE, BMP_WP);
	pieces_white[10] = new Pawn(50, WHITE, BMP_WP);
	pieces_white[11] = new Pawn(51, WHITE, BMP_WP);
	pieces_white[12] = new Pawn(52, WHITE, BMP_WP);
	pieces_white[13] = new Pawn(53, WHITE, BMP_WP);
	pieces_white[14] = new Pawn(54, WHITE, BMP_WP);
	pieces_white[15] = new Pawn(55, WHITE, BMP_WP);
	
	//Update tracker with initial positions
	UpdateTracker();

	//Init movesets
	for (int i = 0; i < 16; i++)
	{
		pieces_white[i]->RecalculateMS();
		pieces_black[i]->RecalculateMS();
	}

	//Initialize marker
	Renderer::SetMarker(32);
}

void GameManager::EndGame()
{
	for (int i = 0; i < 16; i++)
	{
		delete pieces_white[i];
		delete pieces_black[i];
	}
}


void GameManager::AttemptMove()
{
	int move_block = Renderer::GetMarker();
	Piece* move_piece = tracker[selected_block];

	//Assume the move is legal
	bool illegal = false;

	//Check if in moveset
	if (!move_piece->CanMove(move_block))
		return;

	//Temporarily allow move to check legality
	Piece* temp = tracker[move_block];
	tracker[move_block] = move_piece;
	tracker[selected_block] = 0;

	//Kill code... Checks if move block is empty, kills piece if not empty
	if (temp != 0)
		temp->SetAcive(false);

	Piece** opponent = turn ? pieces_black : pieces_white;

	//Check if the move caused any piece from the waiting side 
	//to threaten the king of the playing side 
	illegal = CheckThreats(!turn);

	if (illegal)
	{
		std::cout << "Illegal move" << std::endl;
		
		//Reactivate the piece if it was there
		if (temp != 0)
			temp->SetAcive(true);

		tracker[selected_block] = move_piece;
		tracker[move_block] = temp;

		//Refresh movesets
		CheckThreats(!turn);
		return;
	}

	//Perfrom the move if no voilations
	move_piece->Move(move_block);

	//Update the tracker with the new position
	UpdateTracker();

	DeselectBlock();

	//Update only the two blocks that are effected by the move
	Renderer::ReDrawBlocks(move_block, selected_block, move_piece->GetPixels(), 0);

	//Check if the move by the playing side threatened 
	//the king of the waiting side
	if (CheckThreats(turn))
	{
		std::cout << !turn << " in check " << std::endl;
		std::cout << threat_count << " threat(s) detected " << std::endl;
		if (Checkmate())
			checkmate = true;
	}

	//Change the turn (waiting side is now the playing side
	turn = !turn;
}

bool GameManager::Checkmate()
{
	Piece* threatened_king = turn ? pieces_black[0] : pieces_white[0];
	int x = threatened_king->GetBoardPos().x;
	int y = threatened_king->GetBoardPos().y;
	int o_block = Utils::IndexToBlock(x, y);
	Piece* temp = NULL;

	//Assume checkmate
	bool mate = true;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int c_block = Utils::IndexToBlock(x + i, y + j);

			//Try moving king to the surrounding blocks
			if (threatened_king->CanMove(c_block))
			{
				threatened_king->Move(c_block, true);

				if (tracker[c_block])
				{
					tracker[c_block]->SetAcive(false);
					temp = tracker[c_block];
				}

				UpdateTracker();

				//If any move results in the check being lifted mate = false
				mate = mate && CheckThreats(turn);

				//Move the king back to the orignal block
				threatened_king->Move(o_block, true);

				tracker[c_block] = temp;

				if(temp)
					tracker[c_block]->SetAcive(true);

				CheckThreats(turn);
			}
		}
	}

	UpdateTracker();

	CheckThreats(turn);
	
	if (mate && threat_count == 1)
	{
		Compute_LOS(threats[0]->GetBlock(), o_block);
		if (LOS_Breakable())
			mate = false;
	}

	return mate;
}

bool GameManager::CheckThreats(bool color)
{
	Piece** piece_set = color ? pieces_white : pieces_black;
	threat_count = 0;
	check = false;

	for (int i = 0; i < 16; i++)
	{
		if (!piece_set[i]->GetAcive())
			continue;

		if (piece_set[i]->RecalculateMS())
		{
			check = true;
			threats[threat_count] = piece_set[i];
			std::cout << piece_set[i]->GetType() << " is a threat" << std::endl;
			threat_count++;
		}

	}
	return check;
}

bool GameManager::LOS_Breakable()
{
	Piece** piece_set = !turn ? pieces_white : pieces_black;
	int k = 0;
	bool breakable = false;

	while (LOS[k] != -1)
	{
		for (int i = 0; i < 16; i++)
		{
			breakable = breakable || (piece_set[i]->GetAcive() && piece_set[i]->GetType() != KING && piece_set[i]->CanMove(LOS[k]));
			if ((piece_set[i]->GetAcive() && piece_set[i]->CanMove(LOS[k])))
				std::cout << piece_set[i]->GetColor() << " " << piece_set[i]->GetType() << " can move to " << LOS[k] << std::endl;
		}
		k++;
	}

	std::cout << ((breakable) ? "LOS is breakable" : "LOS not breakable") << std::endl;
	

	return breakable;
}

void GameManager::Compute_LOS(int attacker_pos, int king_pos)
{
	int vert_offset = 8;
	int hor_offset = 1;
	int rdiag_offset = 9;
	int ldiag_offset = 7;

	int offset;

	int diff_pos = king_pos - attacker_pos;

	int counter = 0;

	for (int i = 0; i < 7; i++)
		LOS[i] = -1;

	if (tracker[attacker_pos]->GetType() == KNIGHT || tracker[attacker_pos]->GetType() == PAWN)
	{
		LOS[0] = attacker_pos;
		return;
	}

	if (diff_pos % vert_offset == 0)
		offset = vert_offset;
	else if (diff_pos % rdiag_offset == 0)
		offset = rdiag_offset;
	else if (diff_pos % ldiag_offset == 0)
		offset = ldiag_offset;
	else if (diff_pos % hor_offset == 0)
		offset = hor_offset;

	if (diff_pos < 0)
		offset *= -1;

	for (int i = attacker_pos; i != king_pos; i = i + offset)
	{
		LOS[counter] = i;
		counter++;
	}

	return;
}


Piece ** GameManager::GetTracker()
{
	return tracker;
}

void GameManager::HandleInput()
{
	//Take char input
	char input = _getch();


	switch (input)
	{

	//Update marker position accordingly
	case 'w':
	case 'a':
	case 's':
	case 'd':
		Renderer::UpdateMarker(input);
		break;

	//Refresh screen
	case 'r':
		RefreshScreen();
		break;

	//Quit game
	case 'q':
		quit = true;
		result = QUIT;
		break;

	//Select block
	case ' ':
		if (selected)
		{
			AttemptMove();
			break;
		}

		selected_block = Renderer::GetMarker();
		
		if(!tracker[selected_block])
			break;
		
		if (tracker[selected_block]->GetColor() != turn)
			break;

		SelectBlock();
		break;

	case 'e':
		DeselectBlock();
		return;
	}

}

void GameManager::SelectBlock()
{
	Renderer::MarkBlock(Renderer::GetMarker(), SELECTED_MARKER_COLOR);
	Renderer::SetSelected(selected_block);
	Renderer::SetSelect_Active(true);
	selected = true;
}

void GameManager::DeselectBlock()
{
	Renderer::EraseMarker(selected_block);
	Renderer::SetSelected(-1);
	Renderer::SetSelect_Active(false);
	selected = false;
}

void GameManager::StartMatch()
{
	system("pause");
	system("cls");
	RefreshScreen();

	while (!checkmate && !quit)
	{
		//Check for input without waiting for enter
		if (_kbhit() != 0)
		{
			HandleInput();
			ClearInputStream();
		}
	}		
}

void GameManager::UpdateTracker()
{
	//Clear the tracker
	for (int i = 0; i < 64; i++)
		tracker[i] = 0;

	for (int i = 0; i < 16; i++)
	{
		//Update the tracker by assigning piece at appropriate index
		if(pieces_white[i]->GetAcive())
			tracker[pieces_white[i]->GetBlock()] = pieces_white[i];
		if(pieces_black[i]->GetAcive())
			tracker[pieces_black[i]->GetBlock()] = pieces_black[i];
	}

}

void GameManager::DisplayResult()
{
	if (quit)
		std::cout << "You quit the game" << std::endl;
	else if (checkmate)
	{
		if (!turn)
			std::cout << "CHECKMATE: WHITE WINS!!!!!!" << std::endl;
		else
			std::cout << "CHECKMATE: BLACK WINS!!!!!!" << std::endl;
	}
}