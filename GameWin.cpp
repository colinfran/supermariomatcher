// File: GameWin.cpp
// Author: Colin Franceschini
// Project: CS215 Project 3 Spring 2015
// Description of file contents: This file runs when any type of content changes on the Game window.

#include <afxwin.h>
#include "GameWin.h"
#include <mmsystem.h>


GameWin::GameWin ()
{
	// This function will initialize the game, game window, and certain varoables.
	bool soundPlayed = PlaySound(TEXT("ITSMEMARIO_WAV"), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	CString WindowTitle = "Super Mario Matcher";
	Create (NULL, WindowTitle);
	numRowsCols = 8;
	numBoards = 3;
	myGame.Init(numRowsCols, numBoards);
	firstTime = true;
}

afx_msg void GameWin::OnPaint ()
{
	// This function will display the game onto the window, display the instructuctions, and calls the SetTimer function.
	myGame.Display (this);
	if (firstTime)
	{
		myGame.Instructions(this);
		bool soundPlayed = PlaySound(TEXT("HEREWEGO_WAV"), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		firstTime = false;
		SetTimer(1, 1000, NULL);
	}
}

afx_msg void GameWin::OnLButtonDown( UINT nFlags, CPoint point )
{
	// This function will allow the game to respond to a change in the board.
	myGame.Click (point, this);
	game_status status = myGame.Done();
	if (status != GO)
	{
		myGame.Click(point, this);
		myGame.Message (this);
		myGame.Init(numRowsCols, numBoards);
		bool soundPlayed = PlaySound(TEXT("HEREWEGO_WAV"), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
		Invalidate (TRUE);
	}
}

afx_msg void GameWin::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// This function will allow double clicks.

	OnLButtonDown(nFlags, point);
}


afx_msg void GameWin::OnTimer(UINT nIDEvent)
{
	// This function will call the timer to update in timer every second
	if (nIDEvent == 1)
		myGame.UpdateTime(this);
}

	BEGIN_MESSAGE_MAP (GameWin, CFrameWnd)
	ON_WM_PAINT ()
	ON_WM_LBUTTONDOWN( )
	ON_WM_LBUTTONDBLCLK( )
	ON_WM_TIMER ()
END_MESSAGE_MAP ()
