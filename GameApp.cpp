// File: GameApp.cpp
// Author: Colin Franceschini
// Project: CS215 Project 3 Spring 2015
// Description of file contents: This file runs when variables are initialized in the Init Function and are being initialized in the window.

#include <afxwin.h>
#include "GameApp.h"


BOOL GameApp::InitInstance ()
{
	// This function will initalize the init function when being run in the window.

	m_pMainWnd = new GameWin();
	m_pMainWnd->ShowWindow (m_nCmdShow);
	m_pMainWnd->UpdateWindow ();

	return TRUE;
}

GameApp GameApp;
