#pragma once
#include "DrawController.h"
#include "strings.h"

class MainWindow
{
	HWND hWnd;
	HINSTANCE hInst;
	DrawController* controller;

	HWND hStatus, hTool;
protected:
	HWND createToolbar();
	HWND createStatusBar(int sbid, int cParts);

	static INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void formatWindowText(HWND hWnd, TKEY str_key);


	static LPCWSTR gets(TKEY key);
public:
	MainWindow();
	MainWindow(HINSTANCE hInst);
	ATOM registerWndClass(WNDPROC wndproc);
	BOOL init(int nCmdShow);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	~MainWindow();
};

