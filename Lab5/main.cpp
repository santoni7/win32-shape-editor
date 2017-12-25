// Lab3.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab4.h"
#include "DrawController.h"
#include "strings.h"
#include "MainWindow.h"
#include "CustomTable.h"
#define MAX_LOADSTRING 100
//GLOBAL:
HWND hwndTableDlg;
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

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
		{"cube", IDS_CUBE },
		{"numfmt", IDS_NUMFMT }
		});
}

int APIENTRY wWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	initStrings(hInstance);

	CustomRegister();

	//new MainWindow(hInstance);
	MainRegisterWndClass(hInstance);

	if (!MainInit(nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));
	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) || !IsWindow(hwndTableDlg) || !IsDialogMessage(hwndTableDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CustomUnregister();
	return (int)msg.wParam;
}