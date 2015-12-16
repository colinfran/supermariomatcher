// File: Game.cpp
// Author: Colin Franceschini 
// Project: CS215 Project 3 Spring 2015
// Description of file contents: This file contains the functions that run the game.

#include "Game.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include "Dictionary.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <mmsystem.h>

static CString letters [] = {" ", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s"};
static CBitmap images [NUMIMAGES];
using namespace std;


Game::GameSquare::GameSquare ()
{
	// This function will initialize the variables what and flag.
	what = 0;
	flag = NONE;
}

void Game::GameSquare::Display (CDC * deviceContextP)
{
	// This function will Will display the GameSquare Rectangle containing 
	// the randomized characters. Each character is assigned to a letter

	deviceContextP->Rectangle(where);	

	CDC memDC;
	int res = memDC.CreateCompatibleDC(deviceContextP);
	memDC.SelectObject(&images[what]);
	deviceContextP->TransparentBlt(where.left, where.top, where.Width(), where.Height(), &memDC, 0, 0, 260, 260, SRCCOPY);
	DeleteDC(memDC);
	/* the letters are hidden during gameplay 
	   that is why it is commented out below    
					| | | |
					v v v v					*/
//	deviceContextP->DrawText(CString(letters[what]), where, DT_CENTER);
}

Game::Game ()
{
	// This function will initialize variables used in the game. 
	// It also assigns an image to each letter used in the game square.
	status = GO;
	numRC = 1;
	grid = new GameSquare *[numRC + 2];
	for (int r = 0; r < numRC + 2; r++)
		grid[r] = new GameSquare[numRC + 2];
	int res = bgImageH.LoadBitmap(CString("MARIOBACK_BMP"));
	res = images[1].LoadBitmap(CString("MARIO_BMP"));
	res = images[2].LoadBitmap(CString("LUIGI_BMP"));
	res = images[3].LoadBitmap(CString("BOWERSER_BMP"));
	res = images[4].LoadBitmap(CString("TOAD_BMP"));
	res = images[5].LoadBitmap(CString("YOSHI_BMP"));
	res = images[6].LoadBitmap(CString("GOOMBA_BMP"));
	res = images[7].LoadBitmap(CString("BOO_BMP"));
	res = images[8].LoadBitmap(CString("MUSHROOM_BMP"));
	res = images[9].LoadBitmap(CString("PEACH_BMP"));
	res = images[10].LoadBitmap(CString("KOOPA_BMP"));
	res = images[11].LoadBitmap(CString("BULLETBILL_BMP"));
	res = images[12].LoadBitmap(CString("WALUIGI_BMP"));
	res = images[13].LoadBitmap(CString("BOBOMB_BMP"));
	res = images[14].LoadBitmap(CString("FLYTRAP_BMP"));
	res = images[15].LoadBitmap(CString("STAR_BMP"));
	res = images[16].LoadBitmap(CString("WARIO_BMP"));
	res = images[17].LoadBitmap(CString("CHOMP_BMP"));
	res = images[18].LoadBitmap(CString("LAKITU_BMP"));
	res = images[19].LoadBitmap(CString("SHYGUY_BMP"));	
}

Game::~Game ()
{
	// This function will delete the allocated grid associated with previous gameplay.
	for (int r = 0; r < numRC+2; r++)
		delete [] grid[r];
	delete [] grid;
}

void Game::Init (int R, int B)
{
	// This function will resize the game and reset all required game variables.
//	Dictionary.erase(Dictionary.begin(), Dictionary.end());
	time = 30;
	for (int r = 0; r < numRC+2; r++)
		delete [] grid[r];
	delete [] grid;
	numRC = R;
	grid = new GameSquare *[numRC + 2];
	for (int r = 0; r < numRC + 2; r++)
		grid[r] = new GameSquare[numRC + 2];		
	numBoards = B;
	previousPoints = currentPoints;
	firstClickDone = false;
	FillIn ();
	FindCharacters();
	score = Dictionary.size() * 10;
	status = GO;
	
}

void Game::Instructions (CFrameWnd * windowP)
{
	// This function will automatically print the instruction  message at the beginning
	// of execution.
	CString message = "    Get ready to play the SuperMario Matcher game! \n\n\n";
	message += "    The goal of this game is to match as many Super Mario\n    characters as you can in 30 seconds!\n\n    Click on the 'NEW GAME' button ANYTIME to start a new game \n    Click on the 'END GAME' button to close the window";
	CString title = "Instructions";
	windowP->MessageBox (message, title);
}

void Game::FillIn ()
{
	// This function will fill in the board with random letters that a mario character is assigned to
	for (int r = 1; r <= numRC; r++)
		for (int c = 1; c <= numRC; c++)
		{
			grid[r][c].what = rand() % 19 + 1 ;
		}
}

void Game::Display (CFrameWnd * windowP)
{
	// This function will display all necessary items onto the window.

	CPaintDC dc (windowP);
	CRect rect;
	windowP->GetClientRect (&rect);
	dc.SetBkMode(TRANSPARENT);			
	CDC memDC;
	int res = memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bgImageH);
	dc.TransparentBlt (0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1900, 1000, SRCCOPY); 	
	DeleteDC (memDC);
	SetUp (rect);
	DisplayScoreRect (&dc);
	DisplayNewGameRect(&dc);
	DisplayEndGameRect(&dc);
	DisplayHighestScoreRect(&dc);
	DisplayGameRect (&dc);
	if (time != 0)
		DisplayTimeRect(&dc);
	
	if (time == 0)
		DisplayTimeRect2(&dc);
	
	
}

void Game::SetUp (CRect rect)
{
	// This function will set up all the rectangle boxes in the game window.

	int w = rect.Width();
	int h = rect.Height();
	int halfW = rect.Width() / 2;
	int halfH =  rect.Height() / 2;
	int border = rect.Width() / 10;
	// game rectangle
	gameRect = CRect (rect.left+border*3, rect.top+border*2, halfW+border*2, rect.bottom - border/4);
	// score rectangle
	scoreRect = CRect(rect.left + (w * 3 / 4), rect.top + border * 2, rect.right - border / 3, ((h * 2 / 3) - border / 4));
	// time rectangle & end-of-game rectangle
	if (time == 0)
		timeRect2 = CRect(rect.left + (w * 3 / 4), h * 2 / 3, rect.right - border / 3, rect.bottom - border / 4);
		
	if (time > 0)
		timeRect = CRect(rect.left + (w * 3 / 4), h * 2 / 3, rect.right - border / 3, rect.bottom - border/4);
	// end game rectangle
	endGameRect = CRect(rect.left + border / 3, h * (1 / 3) + border*2, rect.left + (w * 1 / 4), (h * 2 / 3) - border);
	// new game rectangle
	newGameRect = CRect(rect.left + border / 3, (h * 1 / 3) + (border), rect.left + (w * 1 / 4), ((h * 2 / 3) - (border / 3)));
	// highest score rectangle
	HighestScoreRect = CRect(rect.left + border / 3, h * 2 / 3, rect.left + (w * 1 / 4), rect.bottom-border/4);
	int left = gameRect.left;
	int top = gameRect.top; 
	for (int r = 1; r <= numRC; r++)
		for (int c = 1; c <= numRC; c++)
			grid[r][c].where = CRect (left + (c-1) * gameRect.Width()/numRC, top + (r-1) * gameRect.Height()/numRC, left + c * gameRect.Width()/numRC, top + r * gameRect.Height()/numRC);
}

void Game::UpdateTime(CFrameWnd * windowP)
{
	// This function contains the countdown timer. The timer works by decreasing the time by 1 if the time is not 0. 
	// when the timer reaches 0, it stops.
	if (time != 0)
		time--;
		windowP->InvalidateRect(timeRect);
	
}

void Game::UpdateScore(CFrameWnd * windowP)
{
	// This function will update the score every time the score increases
	//score = Dictionary.size() * 10;
//	windowP->InvalidateRect(scoreRect | HighestScoreRect);

}


void Game::DisplayGameRect (CDC * deviceContextP)
{
	// This function will display the game rectangle.
	deviceContextP->Rectangle(gameRect);
	deviceContextP->DrawText("", gameRect, DT_CENTER);
	for (int r = 1; r <= numRC; r++)
		for (int c = 1; c <= numRC; c++)
			grid [r][c].Display (deviceContextP);
}

void Game::DisplayScoreRect (CDC * deviceContextP)
{
	// This function will display the score everytime the score increases.
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(scoreRect.Height(), fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);	
	deviceContextP->Rectangle(scoreRect);
	char scoreStr[20];
	sprintf_s(scoreStr, "%d", score);
	CString score1 = "Score:\n" + CString(scoreStr);
	deviceContextP->DrawText(score1, scoreRect, DT_CENTER);

	deviceContextP->SelectObject(def_font);
}

void Game::DisplayHighestScoreRect(CDC * deviceContextP)
{
	// This function will display the "Higest Score" rectangle
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(HighestScoreRect.Height(), fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);
	deviceContextP->Rectangle(HighestScoreRect);
	char scoreStr[20];
	sprintf_s(scoreStr, "%d", score);
	CString text = "Highest Score:\n" + CString(scoreStr);
	deviceContextP->DrawText(text, HighestScoreRect, DT_CENTER);
	deviceContextP->SelectObject(def_font);
}

void Game::DisplayTimeRect(CDC * deviceContextP)
{
	// This function will display the countdown timer. The timer stops at 0.
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(timeRect.Height(), fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);
	deviceContextP->Rectangle(timeRect);
	char timeStr[20];
	sprintf_s(timeStr, "%d:%02d:%02d", time / 3600, time / 60, time % 60);
	CString text = "Time remaining:\n" + CString (timeStr);
	deviceContextP->DrawText(text, timeRect, DT_CENTER);
	deviceContextP->SelectObject(def_font);
}

void Game::DisplayTimeRect2(CDC * deviceContextP)
{
	// This function will display the "out of time" message telling the user to click on the "New Game" button
	// or click on the "End Game" button.
	
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(timeRect2.Height()/1.25, fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);
	deviceContextP->Rectangle(timeRect2);
	deviceContextP->DrawText("YOU'RE OUT OF TIME!\n GAME OVER!\nPLEASE CLICK ON\n 'NEW GAME'\n OR\n 'END GAME'", timeRect2, DT_CENTER);
	deviceContextP->SelectObject(def_font);
}

void Game::DisplayNewGameRect(CDC * deviceContextP)
{
	// This function will display the "New Game" rectangle/button
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(newGameRect.Height()*4, fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);
	deviceContextP->Rectangle(newGameRect);
	deviceContextP->DrawText("NEW GAME", newGameRect, DT_CENTER);
	deviceContextP->SelectObject(def_font);
}

void Game::DisplayEndGameRect(CDC * deviceContextP)
{
	// This function will display the "End Game" rectangle/button
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(endGameRect.Height()*4, fontName, deviceContextP);
	CFont* def_font = deviceContextP->SelectObject(&font);
	deviceContextP->Rectangle(endGameRect);
	deviceContextP->DrawText("END GAME", endGameRect, DT_CENTER);
	deviceContextP->SelectObject(def_font);
}


void Game::Click (CPoint point, CFrameWnd * windowP)
{
	// This function will check if the players click in the game is valid. If the click is inside the NewGame box
	// the game will restart. If they click the EndGame box, the window will close.
	if (newGameRect.PtInRect(point))
	{
		status = NEW;
		windowP->InvalidateRect(scoreRect | gameRect | timeRect);
	}
	if (endGameRect.PtInRect(point))
	{
		status = QUITTER;
		bool soundPlayed = PlaySound(TEXT("GAMEOVER_WAV"), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	}
	int row = 0; // Calculate row as a function of y
	int col = 0; // Calculate col as a function of x
	for (int r = 1; !row && !col && r <= numRC; r++)
		for (int c = 1; !row && !col && c <= numRC; c++)
			if (grid[r][c].where.PtInRect (point))
			{	
				row = r;
				col = c;
			}
	if (row < 1 || row > numRC || col < 1 || col > numRC)
		return;
	if (time == 0)
		return;
	if (firstClickDone == false)
		FirstClick (row, col, windowP);
	else
		SecondClick (row, col, windowP);
}

void Game::Message (CFrameWnd * windowP)
{
	// This function will display a message when the "New Game" button is clicked
	// and closes the window when the "End Game" button is clicked.
	CString message;
	switch (status)
	{
		case QUITTER:
			Sleep(4000);
			exit(1);
		case NEW:
			message = "Close this window to start the new game";
			Dictionary.erase(Dictionary.begin(), Dictionary.end());
	}
	CString title = "New Game";
	windowP->MessageBox (message, title);
}

game_status Game::Done ()
{
	// This function will return the status of the game.
	return status;
}

bool Game::FirstClick (int row, int col, CFrameWnd * windowP)
{
	// This function will save the location of the user's click
	clickedRow1 = row;
	clickedCol1 = col;
	windowP->InvalidateRect(grid[row][col].where);
	firstClickDone = true;
	return true;
}

bool Game::SecondClick (int row, int col, CFrameWnd * windowP)
{
	// This function will move the characters and update the game
	clickedRow2 = row;
	clickedCol2 = col;
	firstClickDone = false;
	Dictionary.clear();
	windowP->InvalidateRect(gameRect);
	windowP->InvalidateRect(endGameRect);
	windowP->InvalidateRect(newGameRect);
	
	GameSquare temp = grid[clickedRow1][clickedCol1];
	grid[clickedRow1][clickedCol1] = grid[clickedRow2][clickedCol2];
	grid[clickedRow2][clickedCol2] = temp;
	FindCharacters();
	score = Dictionary.size() * 10;
	return true;
}

bool IsCharacterMatch (const CString & maybe)
{
	// This function will check to see if all the matched Mario characters found on the board are real,
	// legal matches. It will return true if it is and false if it isn't.

	int first = 0;
	int last = NUMWORDS - 1;
	while (last - first > 1)
	{
		int mid = (first + last) / 2;
		if (Dictionary[mid] == maybe)
			return true;
		if (maybe < Dictionary[mid])
			last = mid;
		else
			first = mid;
	}
	if (Dictionary[last] == maybe)
		return true;
	if (Dictionary[first] == maybe)
		return true;
	return false;
}

int Game::FindCharacters()
{
	// This function will find all legal matched characters in the current 
	// board. It will look for words that are only in the same column or row.
	CString one = "";
	int count = 0;
	int r;
	int c;
	// VERTICAL DIRECTION
	for (c = 1; c <= numRC; c++)
	{
		for (r = 1; r <= numRC; r++)
		{
			one += letters[grid[r][c].what]; // forwards
		}
		count += FindCharactersInLine(one);
		one = "";
	}
	// HORIZONTAL DIRECTION
	for (r = 1; r <= numRC; r++)
	{
		for (c = 1; c <= numRC; c++)
		{
			one += letters[grid[r][c].what];//forwards
		}
		count += FindCharactersInLine(one);
		one = "";
	}
	return count;
}

int Game::FindCharactersInLine(const CString & line)
{
	// This function checks for real, legal matched characters.
	int count = 0;
	CString ss = "";
	for (int first = 0; first < line.GetLength(); first++)
	{
		for (int len = 0; len < line.GetLength(); len++)
		{
			CString ss = Substring(first, line);
			if (IsCharacterMatch(ss))
			{
				Dictionary.push_back(ss);
			}
			count++;
		}
	}
	return count;
}

CString Game::Substring(int first, const CString & line)
{
	// This function checks the substrings for matched characters
	CString temp;
	for (int i = first; i < line.GetLength(); i++)
	{
		temp += line[i];
	}
	return temp;
}