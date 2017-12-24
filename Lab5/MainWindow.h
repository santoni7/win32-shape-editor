#pragma once
#include "DrawController.h"
#include "strings.h"


ATOM MainRegisterWndClass(HINSTANCE hInst);
BOOL MainInit(int nCmdShow);

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);