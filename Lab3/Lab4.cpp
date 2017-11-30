// Lab3.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab4.h"
#include "shape_editor.h"
#include "strings.h"
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst; // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна
ShapeEditorController* controller;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
void formatWindowText(HWND hWnd, TKEY str_key);
HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
void updateSBSize(int cParts, HWND hwndStatus);

// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
HWND mCreateToolbar(HWND hWndParent);
HBITMAP LoadMyBitmap();
HWND hMainWindow;
HWND hTool;
HWND hStatus;


void initStrings(HINSTANCE hInstance)
{
	strings::instance()->init(hInstance);
	strings::instance()->loadStrings({
		{"status_fmt", IDS_STATUS_FMT},
		{"point", IDS_POINT},
		{"line", IDS_LINE},
		{"ellipse", IDS_ELLIPSE},
		{"rect", IDS_RECTANGLE}
	});
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                           _In_opt_                           HINSTANCE hPrevInstance,
                           _In_                           LPWSTR lpCmdLine,
                           _In_                           int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	initStrings(hInstance);


	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
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


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
auto SB_ID = 514;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	controller = new ShapeEditorController(hWnd);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hTool = mCreateToolbar(hWnd);

	mCreateToolbar(hWnd);

	hStatus = DoCreateStatusBar(hWnd, SB_ID, hInst, 1);


	UpdateWindow(hTool);
	UpdateWindow(hStatus);

	hMainWindow = hWnd;
	updateSBSize(1, hStatus);
	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		controller->OnMouseDown();
		break;
	case WM_LBUTTONUP:
		controller->OnMouseUp();
		break;
	case WM_MOUSEMOVE:
		controller->OnMouseMove();
		break;
	case WM_PAINT:
		controller->OnPaint();
		break;
	case WM_SIZE:
		//updateSBSize(2, hStatus);
		break;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
			case IDM_SHAPETYPE_POINT:
				controller->SetShapeType(ST_POINT);
				controller->SetOutlineColor(RGB(0, 0, 0));
				formatWindowText(hStatus, "point");
				break;
			case IDM_SHAPETYPE_LINE:
				controller->SetShapeType(ST_LINE);
				controller->SetInputMethod(IM_CORNERCORNER);
				controller->SetOutlineColor(RGB(0, 0, 0));
				formatWindowText(hStatus, "line");
				break;
			case IDM_SHAPETYPE_RECTANGLE:
				controller->SetShapeType(ST_RECTANGLE);
				controller->SetInputMethod(IM_CENTERCORNER);
				controller->SetOutlineColor(RGB(0, 0, 0));
				controller->SetFillColor(NULL, false);
				formatWindowText(hStatus, "rect");
				break;
			case IDM_SHAPETYPE_ELLIPSE:
				controller->SetShapeType(ST_ELLIPSE);
				controller->SetInputMethod(IM_CORNERCORNER);
				controller->SetOutlineColor(RGB(0, 0, 0));
				controller->SetFillColor(RGB(255, 165, 0), true);
				formatWindowText(hStatus, "ellipse");
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDM_UNDO:
				controller->Undo();
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		//case WM_PAINT:
		//    {
		//        PAINTSTRUCT ps;
		//        HDC hdc = BeginPaint(hWnd, &ps);
		//        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		//        EndPaint(hWnd, &ps);
		//    }
		//    break;
	case WM_DESTROY:
		controller = NULL;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HIMAGELIST g_hImageList = NULL;

HWND mCreateToolbar(HWND hWndParent)
{
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 4;
	const int bitmapSize = 32;

	const DWORD buttonStyles = TBSTYLE_BUTTON;// BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
	                                  WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS, 0, 0, 0, 0,
	                                  hWndParent, NULL, hInst, NULL);

	if (hWndToolbar == nullptr)
		return nullptr;


	COLORREF mask = RGB(255, 255, 255);
	HIMAGELIST hImageList = ImageList_LoadBitmap(hInst, MAKEINTRESOURCEW(IDB_PT), 32, 0, mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LINE)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RECT)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELLIPSE)), mask);


	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
	            0,//(WPARAM)ImageListID,
	            reinterpret_cast<LPARAM>(hImageList));

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
	            (WPARAM)IDB_STD_SMALL_COLOR,
	            (LPARAM)HINST_COMMCTRL);


	SendMessage(hWndToolbar, TB_SETMAXTEXTROWS,
	            (WPARAM)0, 0);

	//TB_SETMAXTEXTROWS

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{
			0, IDM_SHAPETYPE_POINT, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(strings::instance()->get("point"))
		},
		{
			1, IDM_SHAPETYPE_LINE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(strings::instance()->get("line"))
		},
		{
			2, IDM_SHAPETYPE_RECTANGLE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(strings::instance()->get("rect"))
		},
		{
			3, IDM_SHAPETYPE_ELLIPSE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(strings::instance()->get("ellipse"))
		}
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}

void updateSBSize(int cParts, HWND hwndStatus)
{

	RECT rcClient;
	GetClientRect(hMainWindow, &rcClient);


	// Allocate an array for holding the right edge coordinates.
	auto hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	PINT sb_paParts = static_cast<PINT>(LocalLock(hloc));

	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	int nWidth = rcClient.right / cParts;
	int rightEdge = nWidth;
	for (int i = 0; i < cParts; i++)
	{
		sb_paParts[i] = rightEdge;
		rightEdge += nWidth;
	}

	// Tell the status bar to create the window parts.
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)
			sb_paParts);
}

// Description: 
//   Creates a status bar and divides it into the specified number of parts.
// Parameters:
//   hwndParent - parent window for the status bar.
//   idStatus - child window identifier of the status bar.
//   hinst - handle to the application instance.
//   cParts - number of parts into which to divide the status bar.
// Returns:
//   The handle to the status bar.
//

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE
                       hinst, int cParts)
{
	HWND hwndStatus;
	HLOCAL hloc;
	int i, nWidth;

	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create the status bar.
	hwndStatus = CreateWindowEx(
		0, // no extended styles
		STATUSCLASSNAME, // name of status bar class
		strings::instance()->format("status_fmt", strings::instance()->get("point")), // no text when first created
		//SBARS_SIZEGRIP | // includes a sizing grip
		WS_CHILD | WS_VISIBLE, // creates a visible child window
		0, 0, 0, 0, // ignores size and position
		hwndParent, // handle to parent window
		(HMENU)idStatus, // child window identifier
		hinst, // handle to application instance
		NULL); // no window creation data

	// Get the coordinates of 

	// Allocate an array for holding the right edge coordinates.
	hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	PINT sb_paParts = static_cast<PINT>(LocalLock(hloc));

	updateSBSize(cParts, hwndStatus);

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);
	return hwndStatus;
}


void formatWindowText(HWND hWnd, TKEY str_key)
{
	::SetWindowTextW(hWnd, strings::instance()->format("status_fmt", strings::instance()->get(str_key)));
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
