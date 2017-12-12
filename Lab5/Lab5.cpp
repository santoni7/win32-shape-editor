// Lab3.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab4.h"
#include "DrawController.h"
#include "strings.h"
#include "MainWindow.h"
#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst; // текущий экземпляр

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

MainWindow* wnd;


void initStrings(HINSTANCE hInstance)
{
	strings::instance()->init(hInstance);
	strings::instance()->loadStrings({
		{"wnd_class_name", IDC_LAB3},
		{"app_title", IDS_APP_TITLE},
		{"status_fmt", IDS_STATUS_FMT},
		{"shape_count_fmt", IDS_SHAPECOUNT_FMT },
		{"point", IDS_POINT},
		{"line", IDS_LINE},
		{"ellipse", IDS_ELLIPSE},
		{"rect", IDS_RECTANGLE},
		{"lineoo", IDS_LINEOO },
		{"cube", IDS_CUBE }
	});
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                           _In_opt_                           HINSTANCE hPrevInstance,
                           _In_                           LPWSTR lpCmdLine,
                           _In_                           int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	initStrings(hInstance);

	wnd = new MainWindow(hInstance);
	wnd->registerWndClass(WndProc);

	if (!wnd->init(nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return wnd->WndProc(hWnd, message, wParam, lParam);
}
