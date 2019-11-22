#include "GameManager.h"
using namespace std;

//Handles erroneous input
void ValidateInput()
{

}

int main()
{
	int input;

	//Main menu code/////

	//GameManager manages the entire game/////
	GameManager::Init();
	GameManager::StartMatch();
	GameManager::EndGame();
	GameManager::DisplayResult();
	//Post game menu and loop back to main menu//////


	system("pause");
}
