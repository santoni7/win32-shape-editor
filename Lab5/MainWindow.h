#pragma once
#include "DrawController.h"
#include "strings.h"

class MainWindow
{
	HWND hWnd;
	HINSTANCE hInst;
	DrawController* controller;

	HWND hStatus, hTool;
	static const int SB_PARTS = 2;

protected:
	HWND createToolbar();
	HWND createStatusBar(int sbid, int cParts = SB_PARTS);
	void updateStatusBarSize(int cParts = SB_PARTS);

	static INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void formatWindowText(HWND hWnd, TKEY str_key);
	void formatShapeCountText(int count);


	static LPCWSTR gets(TKEY key);
public:
	MainWindow();
	MainWindow(HINSTANCE hInst);
	ATOM registerWndClass(WNDPROC wndproc);
	BOOL init(int nCmdShow);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	~MainWindow();
};

