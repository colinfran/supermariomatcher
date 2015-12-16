// File: GameWin.h
// Author: Colin Franceschini
// Project: CS215 Project 3 Spring 2015
// Description of file contents: This function declares functions used on the game window.

#include <afxwin.h>
#include "Game.h"

class GameWin : public CFrameWnd
{
	public:
		GameWin ();
		afx_msg void OnPaint ();
		afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point ); 
		afx_msg void OnTimer(UINT nIDEvent);
	private:
		Game myGame;
		bool firstTime;
		int numRowsCols;
		int numBoards;
		DECLARE_MESSAGE_MAP ()
};
