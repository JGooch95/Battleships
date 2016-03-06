/*Programmer: Jonny Gooch
  PNumber: P14141535
  ------Improvements----------
  Make it so the game doesnt crash when a letter is entered in the integer entry.
  ----------------------------                    */

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Initialising Functions
void SaveGame(int iOpponentType, int iCurrentPlayer,  int *iAILastHit, string sAIOrientation, int aiShipHit[2][5], int iCurrentOpponent, string *asPlayerNames);
void LoadGame(int &iCurrentPlayer,int &iOpponentType, int *iAILastHit, string &sAIOrientation, int aiShipHit[2][5], int &iCurrentOpponent, string *asPlayerNames);
void PlayGame(int iOpponentType, int bNewGame);
void SwitchPlayer(int &iCurrentPlayer, int &iCurrentOpponent, string *asPlayerNames);
void DrawVerticalLines();
void DrawGrid(int &iCurrentPlayer,int &iCurrentOpponent, string *asPlayerNames, int aiShipHit[2][5]);
void ClearGrids();
void PlaceShip(string &sOrientation, int &iShipX, int &iShipY, int &i, int &iCurrentPlayer);
bool ShipPlaceValid(string &sOrientation, int &iShipX, int &iShipY, int &i, int &iCurrentPlayer);
void ShipPlacement(int &iCurrentPlayer, int &iOpponentType, int &iCurrentOpponent, string *asPlayerNames, int aiShipHit[2][5]);
void Shoot(int &iCurrentPlayer, int &iOpponentType, int *iAILastHit, string &sAIOrientation, int aiShipHit[2][5], int &iCurrentOpponent, string *asPlayerNames);
bool CheckWin(int &iCurrentPlayer, string *asPlayerNames);
void CheckDestroyed(int iShotX, int iShotY, int &iCurrentPlayer, int &iCurrentOpponent, string *asPlayerNames, bool &bValidShot, bool bShotHit, int aiShipHit[2][5], int *iAILastHit, string sAIOrientation);

//Initialising Global Constants
const int g_kiBoardWidth = 10; //The width of the board.
const int g_kiBoardHeight = 10; //The height of the board.
const string g_aksShipTypes[5] = {"Submarine", "Destroyer", "Cruiser", "Battleship", "Aircraft Carrier"}; //Holds ship names.
const int g_akiShipSizes[5] = {2, 3, 3, 4, 5}; //Holds the sizes of each ship.
const char g_akcShipID[5] = {'S', 'D', 'C', 'B', 'A'}; //Holds the Identification of each ship for the board.

//Initialising Global variables
char g_acShipBoard[g_kiBoardHeight][g_kiBoardWidth][2]; // Holds all of the Players ship positions
char g_acHitBoard[g_kiBoardHeight][g_kiBoardWidth][2]; //Holds all of the Players current guesses

int main()
{
	//Initialising Variables
	int iMenuInput = 0; //Holds the input for the menu interface.
	bool bMainMenuActive = true; //States whether the main menu is being used or not.
	bool bNewGameMenuActive = false; //States whether the new game menu is being used or not.

	while(bMainMenuActive == true) //While the main menu is active
	{
		//Draw main menu interface.
		cout << "Battleships \n\n";
		cout << "1. Continue \n";
		cout << "2. New Game \n";
		cout << "3. Exit \n\n";

		cout << "Which option would you like to select? :  ";

		//Takes the players Main menu input.
		cin >> iMenuInput;

		cout << "\n"; //Draw a new line

		if (iMenuInput == 1) //Continue
		{
			PlayGame(0, false); // Play the game (Opponent type = (loaded), bNewGame = false) 
		}

		else if (iMenuInput == 2) //New Game
		{
			//Activates the new game menu.
			bNewGameMenuActive = true;

			//While the new game menu is active.
			while (bNewGameMenuActive == true)
			{
				//Draw new game menu interface.
				system ("CLS");
				cout << "Please select a Game Mode \n\n";
				cout << "1. Single Player (Player vs CPU) \n";
				cout << "2. Multiplayer (Player vs Player) \n";
				cout << "3. < Back \n\n";

				cout << "Which option would you like to select? :  ";

				//Takes the players input.
				cin >> iMenuInput;

				cout << "\n";
					
				if (iMenuInput == 1) //Single Player
				{
					PlayGame(0, true); // Play the game (Opponent type = 0, bNewGame = true) 
					bNewGameMenuActive = false; // Closes the new game menu
				}

				else if (iMenuInput == 2) //Multiplayer
				{
					PlayGame(2, true); // Play the game (Opponent type = 2, bNewGame = true)
					bNewGameMenuActive = false; // Closes the new game menu
				}

				else if (iMenuInput == 3) //Back
				{
					bNewGameMenuActive = false; // Closes the new game menu
					system ("CLS"); //Clears the screen
				}

				else//Invalid Entry
				{
					cout << "Invalid entry: Please enter a valid value to select an option. \n";
					system ("pause");
					system ("CLS");
				}
			}
		}

		else if (iMenuInput == 3)//Exit
		{
			return 0; //Exits by closing the main loop
		}

		else //Invalid Entry
		{
			cout << "Invalid entry: Please enter a valid value to select an option. \n";
			system ("pause");
			system ("CLS");
		}
	}
}

void SaveGame(int iOpponentType, int iCurrentPlayer,  int *iAILastHit, string sAIOrientation, int aiShipHit[2][5], int iCurrentOpponent, string *asPlayerNames) //Stores variables for later use.
{
	ofstream File1;
	ofstream File2;

	File1.open("Boards.txt", ios::out); //Open File 1 (Contains Boards)

	for (int k = 0; k <= 3; k++)//For every player
	{
		for( int i = 0; i < g_kiBoardWidth; i++)//For every X coordinate
		{
			for( int j = 0; j < g_kiBoardHeight; j++) //For every Y coordinate
			{
				if (k == 0)//Stores ShipBoard for player 1
				{
					File1 << g_acShipBoard[i][j][0];
				}
				else if (k == 1)//Stores HitBoard for player 1
				{
					File1 << g_acHitBoard[i][j][0];
				}
				else if (k == 2) //Stores ShipBoard for player 2
				{
					File1 << g_acShipBoard[i][j][1];
				}
				else if (k == 3) //Stores HitBoard for player 2
				{
					File1 << g_acHitBoard[i][j][1];
				}
			}

			if (!(k == 3 && i == 9)) //for every line besides the last line
			{
				File1 << "\n"; //Save a new line.
			}
		}
	}
	File1.close(); //Close File 1 (Contains Boards)

	File2.open("Stats.txt", ios::out); //Open File 2 (Contains Stats)

	File2 << iOpponentType << "\n"; //Stores the opponent type
	File2 << iCurrentPlayer << "\n"; //Stores the current player
	File2 << iAILastHit[0] << "\n"; //Stores the AI's last successful hit (Y coordinate)
	File2 << iAILastHit[1] << "\n"; //Stores the AI's last successful hit (X coordinate)
	File2 << sAIOrientation << "\n"; //Stores the the AI's last successful orientation
	File2 << iCurrentOpponent << "\n"; //Stores the current opponent.

	for(int i = 0; i < 2; i++) //For every player
	{
		for(int j = 0; j < 5; j++) //For every ship type
		{
			File2 << aiShipHit[i][j] << "\n"; //Store the ship hit value
		}
	}

	for(int i = 0; i < 2; i++) //For every player
	{
		File2 << asPlayerNames[i] << "\n"; //Store the player names.
	}

	File2.close(); //Close File 2 (Contains Stats)
}

void LoadGame(int &iCurrentPlayer,int &iOpponentType, int *iAILastHit, string &sAIOrientation, int aiShipHit[2][5], int &iCurrentOpponent, string *asPlayerNames)
{
	ifstream File1;
	ifstream File2;

	string sLineFromFile; //Stores the line read from the file.

	File1.open("Boards.txt"); //Open File 1 (Contains Boards)

	if (File1.is_open()) //If the file is open.
	{
		for (int k = 0; k <= 3; k++) //For every stored board
		{
			for( int i = 0; i < g_kiBoardWidth; i++) //For every X coordinate.
			{
				getline(File1, sLineFromFile); //Read a line
				for( int j = 0; j < g_kiBoardHeight; j++) //For every Y coordinate.
				{
					if (k == 0) //If the board type is 0
					{
						g_acShipBoard[i][j][0] = sLineFromFile[j]; //Store the contents of the line read in player 1's ShipBoard.
					}
					else if (k == 1) //If the board type is 1
					{
						g_acHitBoard[i][j][0] = sLineFromFile[j]; //Store the contents of the line read in player 1's HitBoard.
					}
					else if (k == 2) //If the board type is 2
					{
						g_acShipBoard[i][j][1] = sLineFromFile[j]; //Store the contents of the line read in player 2's ShipBoard.
					}
					else if (k == 3) //If the board type is 3
					{
						g_acHitBoard[i][j][1] = sLineFromFile[j]; //Store the contents of the line read in player 2's HitBoard.
					}
				}
			}
		}
	}

	else //If the file is not open.
	{
		cout<<"Could not open file."; //Output an error message
	}

	File1.close(); //Close File 1 (Contains Boards)

	File2.open("Stats.txt"); //Open File 2 (Contains Stats)
	
	if (File2.is_open()) //If the file is open.
	{
		getline(File2, sLineFromFile); //Read a line
		iOpponentType = stoi(sLineFromFile); //Stores the contents of the line in Opponent type

		getline(File2, sLineFromFile); //Read a line
		iCurrentPlayer = stoi(sLineFromFile); //Stores the contents of the line in Current Player

		getline(File2, sLineFromFile); //Read a line
		iAILastHit[0] = stoi(sLineFromFile); //Stores the contents of the line in AI Last Hit (Y Coordinate)
			
		getline(File2, sLineFromFile); //Read a line
		iAILastHit[1] = stoi(sLineFromFile); //Stores the contents of the line in AI Last Hit (X coordinate)

		getline(File2, sLineFromFile); //Read a line
		sAIOrientation = sLineFromFile; //Stores the contents of the line in Orientation
		 
		getline(File2, sLineFromFile); //Read a line
		iCurrentOpponent = stoi(sLineFromFile); //Stores the contents of the line in Current Opponent

		for(int i = 0; i < 2; i++) //For every player
		{ 
			for(int j = 0; j < 5; j++) //For the amount of ship types
			{
				getline(File2, sLineFromFile); //Read a line
				aiShipHit[i][j] = stoi(sLineFromFile); //Stores the contents of the line in Ship Hit
			}
		}

		for(int i = 0; i < 2; i++) //For every player
		{
			getline(File2, sLineFromFile); //Read a line
			asPlayerNames[i] = sLineFromFile; //Store the player names.
		}
	}

	else //If the file is not open.
	{
		cout<<"Could not open file."; //Output an error message
	}

	File2.close(); //Close File 2 (Contains Stats)
}

void PlayGame(int iOpponentType , int bNewGame) 
	//iOpponentType States which opponent the player is playing against (0 = CPU, 2 = Player 2)
{
	//Initialises Variables
	int iCurrentPlayer = 0; //States which player is currently playing.
	int iCurrentOpponent = 1; //States the which player is currently the opponent.
	bool bPlayerWin = false; //States whether anyone has won the game.
	int iAILastHit[2] = {-1 , -1}; //Stores the coordinates of where the AI last got a successful hit.
	string sAIOrientation = "none"; //Stores the orientation the AI used last turn.
	int aiShipHit[2][5] = {{0,0,0,0,0} , {0,0,0,0,0}}; // Stores the amount of times each ship has been hit to determine if a ship has been destroyed.
	string asPlayerNames[2]; //Holds the names of both players.

	if (bNewGame) // If the game is a new game.
	{
		cin.ignore( numeric_limits <streamsize> ::max(), '\n'); //Ignores the cin so the getline command works correctly.

		//Allows each player to enter their name
		for (int i = 0; i < 2; i++) //loops through both players
		{
			cout << "Please enter your name : ";  //Prompts the player to enter their name
			getline(cin, asPlayerNames[i]); //Takes the input and stoees it in the player Names array.
			cout << "\n"; // Enters a new line.
		}

		//Both of the players place their ships.
		ShipPlacement(iCurrentPlayer, iOpponentType, iCurrentOpponent, asPlayerNames, aiShipHit);
		ShipPlacement(iCurrentPlayer, iOpponentType, iCurrentOpponent, asPlayerNames, aiShipHit);
	}

	else 
	{
		LoadGame(iCurrentPlayer,iOpponentType, iAILastHit, sAIOrientation,aiShipHit, iCurrentOpponent, asPlayerNames); //The game is loaded.
	}

	while (bPlayerWin == false) //While there is no winner.
	{
		SaveGame(iOpponentType, iCurrentPlayer, iAILastHit, sAIOrientation, aiShipHit, iCurrentOpponent, asPlayerNames); //Save the game
		system("CLS"); //Clear the screen

		//If the player is not AI then draw the grid.
		if (!(iCurrentPlayer == 1 && iOpponentType == 0))
		{
			DrawGrid(iCurrentPlayer,iCurrentOpponent, asPlayerNames, aiShipHit);
		}
		
		Shoot(iCurrentPlayer,iOpponentType, iAILastHit, sAIOrientation, aiShipHit, iCurrentOpponent, asPlayerNames); //Allow the player to shoot.

		bPlayerWin = CheckWin(iCurrentPlayer, asPlayerNames); //Check if the player has won.
	}
}

void SwitchPlayer(int &iCurrentPlayer, int &iCurrentOpponent, string *asPlayerNames) //Switches the player playing.
{
	swap(iCurrentPlayer, iCurrentOpponent); //Swaps currrent player and current Opponent.
	system("CLS"); //Clear the screen.
	cout << "It's now " << asPlayerNames[iCurrentPlayer] << "'s" << " turn." << "\n"; //State who's about to play.
	system("pause"); //Wait for player input.
}

void DrawGrid(int &iCurrentPlayer,int &iCurrentOpponent, string *asPlayerNames, int aiShipHit[2][5]) //Draws the player grids to the screen.
{
	int bShipDestroyed = false; //Stores whether the whole ship is destroyed.

	//Outputs the title to the screen.
	cout << "Battleships\n\n";

	//Outputs the Player currently being shown.
	cout << "Current Player - " << asPlayerNames[iCurrentPlayer] << "\n\n";

	//(k == 0) = Draws the Shipboard.
	//(k == 1) = Draws the Hitboard.
	for (int k = 0; k < 2; k++)
	{
		//Outputs the title of the board to the screen
		if (k == 0)
		{
			cout << "ShipBoard \n\n";
		}
		else if (k == 1)
		{
			cout << "\nHitBoard\n\n";
		}

		//Writes the UI to display the X positions of the grid
		cout << "   "; //outputs a margin to move the X values to the correct position.

		//For every space on the top of the grid write its number value.
		for(int i = 0; i !=g_kiBoardWidth; i++)
		{
			cout << left << setw(4) << i + 1 ;
		}

		cout << '\n' << "  "; //Places a margin to correctly position the line.

		// For every space in the x direction.
		for(int i = 0; i !=g_kiBoardWidth; i++)
		{
			cout << "----"; //Place part of the line
		}
		

		cout << right << "\n"; //Resets the values positions to the right then moves down a line.

		//Draws the grid spaces and the y axis.
		for (int i = 0; i < g_kiBoardHeight; i++)
		{		
			cout << setw(2) << char((i + 1) + 64); //Writes the UI to display the Y positions of the grid
		
			//Enters the data into the grid spaces.
			for(int j = 0; j < g_kiBoardWidth; j++)
			{
				if (k == 0)
				{
					cout << setw(2) << g_acShipBoard[i][j][iCurrentPlayer] << setw(2); //Draws the Ship values into each of the grid spaces.
				}

				else if (k == 1)
				{
					for (int l = 0; l < sizeof(g_aksShipTypes)/sizeof(*g_aksShipTypes); l++) //For all ships
					{
						if(aiShipHit[iCurrentPlayer][l] != g_akiShipSizes[l] && g_acHitBoard[i][j][iCurrentPlayer] == g_akcShipID[l]) //If the ship is not fully hit or the shot is a miss.
						{ 
							bShipDestroyed = false; //Whole ship is not hit.
							l = sizeof(g_aksShipTypes)/sizeof(*g_aksShipTypes); //Breaks the for loop.
						}
						else  //If the whole ship is hit
						{
							bShipDestroyed = true;//The whole ship is hit.
						}
					}

					if (bShipDestroyed == false) //If the ship isn't destroyed.
					{
						cout << setw(2) << 'X' << setw(2); //Draws the Hit values into each of the grid spaces.
					}
					else //If the ship is destroyed.
					{
						cout << setw(2) << g_acHitBoard[i][j][iCurrentPlayer] << setw(2); //Draws the destroyed ship into each of the grid spaces.
					}
				}
			
				//Places a divider after each value except the last one on each row to divide the grid.
				if (j != g_kiBoardWidth-1)
				{
					cout << "|";
				}
			}

			cout <<"\n"; //Goes to a new line.

			//Places horizontal dividers to divide the rows.
			if (i <= g_kiBoardHeight - 1)
			{
				cout << "  "; //Places a margin to correctly position the line.

				// For every space in the x direction.
				for(int i = 0; i !=g_kiBoardWidth; i++)
				{
					cout << "----"; //Place part of the line
				}
				cout << "\n"; //Go to a new line.
			}
		}
	}
	cout << "\n"; //Go to a new line.
}

void ClearGrids() //Clears both of the players grids.
{ 
	for(int i = 0; i < g_kiBoardWidth; i++) //For all of the X values.
	{
		for(int j = 0; j < g_kiBoardHeight; j++) //For all of the Y values.
		{
			for(int k = 0; k < 2; k++) //For both players.
			{
				g_acShipBoard[i][j][k] = '\0'; //Make the coordinate on the ship board blank.
				g_acHitBoard[i][j][k] = '\0'; //Make the coordinate on the hit board blank.
			}
		}
	}
}

void PlaceShip(string &sOrientation, int &iShipX, int &iShipY, int &i, int &iCurrentPlayer, char cShipID) //Places the ship onto the board.
{
	//Resets the X or Y coordinate back to the starting coordinate so the ship can be placed.
	if(sOrientation == "down")
	{
		iShipY -= g_akiShipSizes[i];
	}
	if(sOrientation == "up")
	{
		iShipY += g_akiShipSizes[i];
	}
	if(sOrientation == "right")
	{
		iShipX -= g_akiShipSizes[i];
	}
	if(sOrientation == "left")
	{
		iShipX += g_akiShipSizes[i];
	}

	//For each unit of the ship...
	for(int j = 0; j < g_akiShipSizes[i]; j++)
	{
		//The unit is placed in the coordinate position.
		g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] = cShipID;

		//The X / Y position is then incremented / decremented according to the orientation.
		if(sOrientation == "down")
		{
			iShipY++;
		}
		if(sOrientation == "up")
		{
			iShipY--;
		}
		if(sOrientation == "right")
		{
			iShipX++;
		}
		if(sOrientation == "left")
		{
			iShipX--;
		}
	}
}

bool ShipPlaceValid(string &sOrientation, int &iShipX, int &iShipY, int &i, int &iCurrentPlayer) //Checks if the player entry is valid.
{
	int iValidCount = 0; //Holds a counter to check whether all of the spaces for the ship are free.
	
	for(int j = 0; j < g_akiShipSizes[i]; j++) //For each unit of the ship...
	{
	
		//If the coordinate is currently not taken.
		if (g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] != 'S' && 
			g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] != 'D' &&
			g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] != 'C' &&
			g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] != 'B' &&
			g_acShipBoard[iShipY - 1][iShipX - 1][iCurrentPlayer] != 'A'   )
		{
			if(iShipY - 1 >= 0 && iShipY - 1 <= g_kiBoardWidth && iShipX - 1 >= 0 && iShipX - 1 <= g_kiBoardHeight) //If the coordinate is within the boundaries.
			{
				iValidCount++; //The counter is increased.
			}
			else
			{
				cout <<"Invalid entry: This orientation positions the ship outside of the board. \n"; //Display error message.
			}
		}

		//The ships X or Y coordinate is Incremented / Decremented depending on orientation so the next coordinate can be checked.
		if(sOrientation == "down")
		{
			iShipY++;
		}
		if(sOrientation == "up")
		{
			iShipY--;
		}
		if(sOrientation == "right")
		{
			iShipX++;
		}
		if(sOrientation == "left")
		{
			iShipX--;
		}
	}

	if (iValidCount == g_akiShipSizes[i]) //If the counter matches the size of the ship.
	{
		return true;  //Return the ship can be placed.
	}
	else
	{
		return false; //Return the ship cannot be placed.
	}
}

void ShipPlacement(int &iCurrentPlayer, int &iOpponentType, int &iCurrentOpponent, string *asPlayerNames, int aiShipHit[2][5]) //Prompts the player to place their ships.
{
	//Initialising Variables
	int iShipY = -1; // Holds the Y coordinate for the current ship.
	int iShipX = -1; // Holds the X coordinate for the current ship.
	bool bOrientationValid = false; //States if the orientaion of the ship is possible.
	string sOrientation = "None"; //Holds the orientation of the ship.
	int iRandOrientation; // Used for the AI to randomise the orientation values.
	char cShipYEntry; //Takes the character value for the x axis to be converted.
	
	if (iCurrentPlayer == 0 || iOpponentType == 2)// If the player is human
	{
		for(int i = 0; i < (sizeof(g_aksShipTypes)/sizeof(*g_aksShipTypes)); i++) //For every ship...
		{
			system("CLS"); //Clears the screen
			DrawGrid(iCurrentPlayer,iCurrentOpponent, asPlayerNames, aiShipHit); //Draws the grid ready for the ship to be placed.

			bOrientationValid = false; //The orientation becomes invalid.

			//Resets the placement variables.
			sOrientation = "None"; 
			iShipY = -1;
			iShipX = -1;

			//While the required positioning variables are invalid...
			while (iShipY - 1 >= g_kiBoardWidth || iShipY- 1 < 0 || iShipX- 1 >= g_kiBoardHeight || iShipX- 1 < 0 || bOrientationValid == false)
			{
				//X Input
				while (iShipX - 1 >= g_kiBoardWidth || iShipX - 1 < 0) //While X is not on the board
				{
					//Asks the player to enter an x coordinate.
					cout << "\n" << "Please enter the x coordinate of your " << g_aksShipTypes[i] << " (Size: " << g_akiShipSizes[i] 
						 << ", 1 - " << g_kiBoardWidth << ") : ";

					cin >> iShipX; //Takes the players input.

					if (iShipX - 1 >= g_kiBoardWidth || iShipX - 1 < 0) //if the entry is not on the board.
					{
						cout << "\nInvalid entry: (Please put a coordinate between 1 and " << g_kiBoardWidth <<").\n"; //Output an error message.
					}
				}

				//Y Input
				while (iShipY- 1 >= g_kiBoardHeight || iShipY - 1 < 0) //While Y is not on the board
				{
					//Asks the player to enter a Y coordinate.
					cout << "\n" << "Please enter the y coordinate of your " << g_aksShipTypes[i] << " (Size: " << g_akiShipSizes[i] << ") "
						 << ", A - " << char(g_kiBoardHeight + 64) << ") : ";

					cin >> cShipYEntry; //Takes the players input.

					cShipYEntry = toupper(cShipYEntry); //Makes the players entry uppercase.

					if (cShipYEntry >= 'A' && cShipYEntry <= 'Z') //If the entry is a valid letter.
					{
						iShipY = int(cShipYEntry) - 64; //Convert it to its integer value.
					}
					
					if (iShipY - 1 >= g_kiBoardHeight || iShipY - 1 < 0)//if the entry is not on the board.
					{
						cout << "\nInvalid entry: (Please put a coordinate between A and " << char(g_kiBoardHeight + 64) <<").\n"; //Output an error message.
					}
				}

				//Orientation input
				cout << "\n" << "Please enter the orientation of the ship. (Up, Down, Left or Right) : "; //Asks the player to enter an orientation.

				cin >> sOrientation; //Takes the players input.

				for (int k = 0; k < sOrientation.length(); k++) //For every letter in the string.
				{
					sOrientation[k] = tolower(sOrientation[k]); //Makes the players entry lowercase.
				}

				if (sOrientation == "left" || sOrientation == "right" || sOrientation == "up" || sOrientation == "down") //If the Orientation entered is valid input. 
				{
					if (ShipPlaceValid(sOrientation, iShipX, iShipY, i, iCurrentPlayer) == true) //If all of the coordinates are empty
					{
						PlaceShip(sOrientation, iShipX, iShipY, i, iCurrentPlayer, g_akcShipID[i]); //Place the ship.
						bOrientationValid = true; //The orientation is stated as valid
					}

					//If the space is not available but the ship is still within the boundaries.
					else if(iShipY - 1 >= 0 && iShipY - 1 <= g_kiBoardWidth && iShipX - 1 >= 0 && iShipX - 1 <= g_kiBoardHeight) 
					{
						cout << "Invalid entry:, This ship intersects another ship. \n"; //Output an error message.
						bOrientationValid = false; //The orientation is stated as invalid
					}
				}

				else //If the orientation user input was not entered correctly...
				{
					cout << "Invalid entry:, Please input either Up, Down,Left or Right.  \n"; //Output an error message.

					bOrientationValid = false; //The orientation becomes invalid.

					//Resets the placement variables.
					sOrientation = "None"; 
					iShipY = -1;
					iShipX = -1;
				}

				//If any of the data entered was invalid the variables which are needed are all reset.
				if(iShipY - 1 >= g_kiBoardWidth || iShipY - 1 < 0 || iShipX - 1 >= g_kiBoardHeight || iShipX - 1 < 0 || bOrientationValid == false)
				{
					bOrientationValid = false;//The orientation becomes invalid.

					//Resets the placement variables.
					sOrientation = "None";
					iShipY = -1;
					iShipX = -1;
				}
			}
		}
	}

	else if (iCurrentPlayer == 1 && iOpponentType == 0) //If the player is the CPU.
	{
		srand (time(NULL)); //Initialise the randomiser.

		for(int i = 0; i < (sizeof(g_aksShipTypes)/sizeof(*g_aksShipTypes)); i++) //For every ship...
		{
			bOrientationValid = false;//The orientation becomes invalid.

			//Resets the placement variables.
			sOrientation = "None";
			iShipY = -1;
			iShipX = -1;

			while (bOrientationValid == false) //While the placement isn't valid.
			{
				iShipY = (rand() % g_kiBoardHeight)  + 1;  //Randomise an Y coordinate between 1 and the board Height
				iShipX = (rand() % g_kiBoardWidth)  + 1;   //Randomise an X coordinate between 1 and the board Width
				iRandOrientation = (rand() % 4); //Randomises a number to represent the orientation of the ship.
				
				//Sets the Orientation dependant on the randomised value.
				if (iRandOrientation == 0) 
				{
					sOrientation = "right";
				}
				else if (iRandOrientation == 1)
				{
					sOrientation = "left";
				}
				else if (iRandOrientation == 2)
				{
					sOrientation = "up";
				}
				else if (iRandOrientation == 3)
				{
					sOrientation = "down";
				}

				//Moves the ship into the board if the ships orientation places the ship outside the board.
				if(sOrientation =="right" && (iShipX + g_akiShipSizes[i]) > g_kiBoardWidth) //If the ship is outside the right side of the board.
				{
					iShipX -= (iShipX  + g_akiShipSizes[i] - g_kiBoardWidth - 1); //Move the ship left by its size.
				}

				if(sOrientation =="left" && iShipX < 0) //If the ship is outside the left side of the board.
				{
					iShipX = 0; //Move the ship to the left edge of the board.
				}
				if(sOrientation =="up" && iShipY <0) //If the ship is outside the top side of the board.
				{
					iShipY = 0; //Move the ship to the top edge of the board.
				}
				if(sOrientation =="down" && (iShipY + g_akiShipSizes[i]) > g_kiBoardHeight) //If the ship is outside the bottom side of the board.
				{
					iShipY -= (iShipY  + g_akiShipSizes[i] - g_kiBoardHeight - 1); //Move the ship up by its size.
				}
				
				if (ShipPlaceValid(sOrientation, iShipX, iShipY, i, iCurrentPlayer) == true) //If the ships placement is valid.
				{
					bOrientationValid = true; //The orientation is declared as valid
					PlaceShip(sOrientation, iShipX, iShipY, i, iCurrentPlayer, g_akcShipID[i]); //The ship is placed
				}

				//If any of the data entered was invalid the variables which are needed are all reset.
				if(bOrientationValid == false)
				{
					bOrientationValid = false; //The orientation becomes invalid.

					//Resets the placement variables.
					sOrientation = "None";
					iShipY = -1;
					iShipX = -1;
				}
			}
		}	
	}

	SwitchPlayer(iCurrentPlayer, iCurrentOpponent, asPlayerNames); //The player is switched.
}

void Shoot(int &iCurrentPlayer, int &iOpponentType, int *iAILastHit, string &sAIOrientation, int aiShipHit[2][5], int &iCurrentOpponent, string *asPlayerNames)//Allows the current player to take a shot and detects if they have hit.
{
	//Initialising variables.
	int iShotX = -1; //Stores the X coordinate of the shot taken.
	int iShotY = -1; //Stores the Y coordinate of the shot taken.
	bool bValidShot = false; //States whether the shot taken is valid.
	char cShotYEntry; // Stores the Y coordinate as a character.
	int iRandOrientation; //Stores a randomised number for the orientation.
	bool bShotHit = false; //Stores whether the shot has hit anything or not.

	if (iCurrentPlayer == 0 || iOpponentType == 2) //If the player is human
	{
		while(bValidShot == false) //While the shot is not valid
		{
			//Prompts the player to enter an x coordinate until it is within the board boundaries.
			while(iShotX < 0 || iShotX > g_kiBoardWidth) //While the shot is not within the board.
			{
				cout << "Please enter the X coordinate you wish to attack "<< " (1 - " << g_kiBoardWidth << ") : "; //Prompts the player to enter an x coordinate.
				cin >> iShotX; //Takes the player's input
				cout << "\n"; //Goes to a new line.

				if (iShotX < 0 || iShotX > g_kiBoardWidth) //If the shot is not within the board
				{
					cout << "Invalid entry: The X value you have entered is outside of the board. \n"; //Diaplays an error message
				}
			}

			//Prompts the player to enter a y coordinate until it is within the board boundaries.
			while(iShotY < 0 || iShotY > g_kiBoardHeight) //While the shot is not within the board.
			{
				cout << "Please enter the Y coordinate you wish to attack "<< "( A - " << char(g_kiBoardHeight + 64) << ") : "; //Prompts the player to enter a y coordinate.
				cin >> cShotYEntry; //Takes the player's input

				cShotYEntry = toupper(cShotYEntry); //Makes the user input uppercase to make any entry the same format.
				if (cShotYEntry >= 'A' && cShotYEntry <= 'Z') // If the character entered was in the valid range.
				{
					iShotY = int(cShotYEntry) - 64; //The character is converted to its integer counterpart.
				}
				cout << "\n"; //Writes a new line.

				if (iShotY < 0 || iShotY > g_kiBoardHeight) //If the shot is outside of the board.
				{
					cout << "Invalid entry: The Y value you have entered is outside of the board. \n"; // Display an error message.
				}
			}

			//If the location on the board has not been guessed before then...
			if (g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'M' && 
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'X' && 
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'S' && 
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'D' &&
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'C' &&
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'B' &&
				g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'A' )
			{
				CheckDestroyed(iShotX, iShotY, iCurrentPlayer, iCurrentOpponent, asPlayerNames, bValidShot, bShotHit, aiShipHit, iAILastHit, sAIOrientation);
			}

			else //If the shot has already been taken
			{
				cout << "Invalid entry: You have already guessed that space. \n"; //Display an error message.
				iShotX = -1; //Reset the shots X value.
				iShotY = -1; //Reset the shots Y value.
			}
		}
	}

	else if (iCurrentPlayer == 1 && iOpponentType == 0) //If the player is the CPU.
	{
		srand (time(NULL)); //Initialise the randomiser.

		while (bValidShot == false) //While the shot is not valid
		{
			if (iAILastHit[0] != -1 && iAILastHit[1] != -1) //If there is a last hit stored.
			{
				iShotX = iAILastHit[1]; //The X coordinate of the shot becomes the last successful hit coordinate.
				iShotY = iAILastHit[0]; //The Y coordinate of the shot becomes the last successful hit coordinate.

				if (sAIOrientation != "up" && sAIOrientation != "down" && sAIOrientation != "left" && sAIOrientation != "right") //If there is no stored orientation.
				{
					iRandOrientation = (rand() % 4); //Randomise a number to represent the orientation.
		
					if (iRandOrientation == 0) //0 = Right
					{
						sAIOrientation = "right"; 
					}
					else if (iRandOrientation == 1) //1 = Left
					{
						sAIOrientation = "left"; 
					}
					else if (iRandOrientation == 2) //2 = Up
					{
						sAIOrientation = "up";
					}
					else if (iRandOrientation == 3) //3 = Down
					{
						sAIOrientation = "down";
					}
				}

				//Increments the X value by the Orientation set.
				if (sAIOrientation == "right") //Right increases the x coordinate by 1.
				{
					iShotX++;
				}
				else if (sAIOrientation == "left")//Left decreases the x coordinate by 1.
				{
					iShotX--;
				}
				else if (sAIOrientation == "up")//Up decreases the y coordinate by 1.
				{
					iShotY--;
				}
				else if (sAIOrientation == "down")//Down increases the y coordinate by 1.
				{
					iShotY++;
				}
			}

			else //If there is not a coordinate stored.
			{
				iShotX = (rand() % g_kiBoardWidth) + 1; //Randomise a X coordinate for the shot within the board boundaries.
				iShotY = (rand() % g_kiBoardHeight) + 1; //Randomise a Y coordinate for the shot within the board boundaries.
			}

			if (g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'M' && g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] != 'X')
			{
				CheckDestroyed(iShotX, iShotY, iCurrentPlayer, iCurrentOpponent, asPlayerNames, bValidShot, bShotHit, aiShipHit, iAILastHit, sAIOrientation);
			}
		}
	}
}

bool CheckWin(int &iCurrentPlayer, string *asPlayerNames) //Checks to see if the current player has won.
{
	//Initialising variables
	int iBoardValidCount = 0; // Stores the amount of X's currently on the board.
	int iCheckCounter = 0; //Stores the amount of units to be hit.

	for(int i = 0; i < g_kiBoardHeight; i++) //For all of the X values.
	{
		for(int j = 0; j < g_kiBoardWidth; j++) //For all of the Y values.
		{
			//If the space has a hit within it...
			if (g_acHitBoard[i][j][iCurrentPlayer] == 'S' || 
				g_acHitBoard[i][j][iCurrentPlayer] == 'D' ||
				g_acHitBoard[i][j][iCurrentPlayer] == 'C' ||
				g_acHitBoard[i][j][iCurrentPlayer] == 'B' ||
				g_acHitBoard[i][j][iCurrentPlayer] == 'A'    )
			{
				iBoardValidCount++; //The Validation counter is incremented.
			}
		}
	}

	for(int i = 0; i < sizeof(g_akiShipSizes)/sizeof(*g_akiShipSizes); i++) //For all of the X values.
	{
		iCheckCounter += g_akiShipSizes[i];
	}
	
	if (iBoardValidCount == iCheckCounter) //If all of the hits add up to the ship spaces taken...
	{
		system("CLS"); //Clear the screen.
		cout << asPlayerNames[iCurrentPlayer] << " Wins. \n";//Output which player wins
		system("pause"); //Wait for player input
		system("CLS"); //Clear the screen.
		ClearGrids(); //Clear the grids.
		return true; // Return the game has been won.
	}

	else
	{
		return false; //Return the game has not been won
	}
}

void CheckDestroyed(int iShotX, int iShotY, int &iCurrentPlayer, int &iCurrentOpponent, string *asPlayerNames, bool &bValidShot, bool bShotHit, int aiShipHit[2][5], int *iAILastHit, string sAIOrientation)
//Checks that the player has destroyed a ship if so a message is output.
{	
	bValidShot = true; //The shot becomes valid
	int iShipHit = 0;
	for(int i = 0; i < (sizeof(g_aksShipTypes)/sizeof(*g_aksShipTypes)); i++) //For every ship type.
	{
		if (g_acShipBoard[iShotY-1][iShotX-1][iCurrentOpponent] == g_akcShipID[i]) //If the ship is in the location
		{
			bShotHit = true; //The shot was a hit.
			iShipHit = i;
			aiShipHit[iCurrentPlayer][i] += 1; // The ship hit counter for this ship is incremented.
			if (aiShipHit[iCurrentPlayer][i] == g_akiShipSizes[i] ) //If all of the units of this ship are hit.
			{
				system("cls");//Clear the screen.
				cout << asPlayerNames[iCurrentPlayer] << " has destroyed a " << g_aksShipTypes[i] << "\n"; //Display ship destruction message.
				system("pause");//Wait for player input.
			}
		}
	}

	if (bShotHit == true) //If the shot did hit.
	{
		g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] = g_akcShipID[iShipHit]; //The current players hitboard displays a hit on the coordinate.
		g_acShipBoard[iShotY-1][iShotX-1][iCurrentOpponent] = '0';//The current opponents shipboard displays a hit on the coordinate.
		iAILastHit[0] = iShotY; //The last succesful hit is recorded (Y coordinate)
		iAILastHit[1] = iShotX; //The last succesful hit is recorded (X coordinate)
		bShotHit = false;
	}

	else //If the shot missed.
	{
		g_acHitBoard[iShotY-1][iShotX-1][iCurrentPlayer] = 'M'; //The current players hitboard displays a miss on the coordinate.
		g_acShipBoard[iShotY-1][iShotX-1][iCurrentOpponent] = 'M'; //The current opponents shipboard displays a miss on the coordinate.
		sAIOrientation = "none"; //The shot orientation is reset.
		iAILastHit[1] = -1; //The last succesful hit variable is reset (Y coordinate) 
		iAILastHit[0] = -1; //The last succesful hit variable is reset (X coordinate) 
		SwitchPlayer(iCurrentPlayer, iCurrentOpponent, asPlayerNames); //The current player is switched.
	}
}

void DrawVerticalLines() //Places a vertical line to split the data apart.
{
	cout << "  "; //Fills the space between the vertical lines of the grid.

	//Places a vertical line for every division of the grid.
	for(int i = 0; i !=g_kiBoardWidth-1; i++)
	{
		cout << setw(6) << "|";
	}

	cout << "\n"; //Goes to a new line.
}