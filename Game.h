// File: Game.h
// Author: Colin Franceschini
// Project: CS215 Project 3 Spring 2015
// Description of file contents: This file will declare functions and variables used in the game.

#ifndef GAME_H
#define GAME_H

#include <afxwin.h>

#include <vector>
#include <string>

using namespace std;

#define NUMIMAGES 20
#define NUMMASKS 20

enum flag_types {NONE, SELECTED, USED};
enum game_status { GO, QUITTER, LOSER, WINNER, NEW};

class Game
{
	public:
		Game ();
		Game (const Game & other);
		~Game ();
		Game & operator = (const Game & other);
		void Init (int RC, int B);
		void Instructions (CFrameWnd * windowP);
		void Display (CFrameWnd * windowP);
		void Click (CPoint point, CFrameWnd * windowP);
		void Message (CFrameWnd * windowP);
		void UpdateTime(CFrameWnd * windowP);
		void UpdateScore(CFrameWnd * windowP);
		
		game_status Done ();
	private:
		struct GameSquare
		{
			GameSquare ();
			void Display (CDC * deviceContextP);
			int what;
			flag_types flag;
			CRect where;
		};

		void FillIn ();
		void SetUp (CRect window);
		void DisplayGameRect (CDC * deviceContextP);
		void DisplayScoreRect (CDC * deviceContextP);
		void DisplayTimeRect(CDC * deviceContextP);
		void DisplayTimeRect2(CDC * deviceContextP);
		void DisplayHighestScoreRect(CDC * deviceContextP);
		void DisplayNewGameRect(CDC * deviceContextP);
		void DisplayEndGameRect(CDC * deviceContextP);
		bool FirstClick (int row, int col, CFrameWnd * windowP);
		bool SecondClick (int row, int col, CFrameWnd * windowP);
		int FindCharacters();
		int FindCharactersInLine(const CString & line);

		CString Substring(int start, const CString & line);
		game_status status;
		GameSquare ** grid;
		int numRC;
		int numBoards;
		int time;
		int time2;
		int score;
		int clickedRow1, clickedCol1;
		int clickedRow2, clickedCol2;
		bool firstClickDone;
		int currentPoints, previousPoints;

		vector <CString> Dictionary;
		CRect gameRect;
		CRect timeRect;
		CRect timeRect2;
		CRect scoreRect;
		CRect endGameRect;
		CRect newGameRect;
		CRect HighestScoreRect;


		CBitmap bgImageH;
};

#endif
