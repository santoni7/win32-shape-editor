#include "stdafx.h"
#include "MainWindow.h"
#include "resource.h"
#include "strings.h"


MainWindow::MainWindow()
{
}

MainWindow::MainWindow(HINSTANCE hInstance)
{
	this->hInst = hInstance;
}

ATOM MainWindow::registerWndClass(WNDPROC wndproc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndproc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = gets("wnd_class_name");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL MainWindow::init(int nCmdShow)
{
	hWnd = CreateWindowW(gets("wnd_class_name"), gets("app_title"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr,
		hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	controller = new DrawController(hWnd);

	hTool = createToolbar();

	const int SB_ID = 1001;
	hStatus = createStatusBar(SB_ID, 1);

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	return TRUE;
}


HWND MainWindow::createToolbar()
{
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 4;
	const int bitmapSize = 32;

	const DWORD buttonStyles = TBSTYLE_BUTTON;// BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
	                                  WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS, 0, 0, 0, 0,
	                                  hWnd, NULL, hInst, NULL);

	if (hWndToolbar == nullptr)
		return nullptr;


	COLORREF mask = RGB(255, 255, 255);
	HIMAGELIST hImageList = ImageList_LoadBitmap(hInst, MAKEINTRESOURCEW(IDB_PT), bitmapSize, 0, mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LINE)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RECT)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELLIPSE)), mask);


	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
	            WPARAM(ImageListID),
	            LPARAM(hImageList));

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
	            WPARAM(IDB_STD_SMALL_COLOR),
	            LPARAM(HINST_COMMCTRL));

	SendMessage(hWndToolbar, TB_SETMAXTEXTROWS,
	            WPARAM(0), 0);

	TBBUTTON tbButtons[numButtons] =
	{
		{
			0, IDM_SHAPETYPE_POINT, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(gets("point"))
		},
		{
			1, IDM_SHAPETYPE_LINE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(gets("line"))
		},
		{
			2, IDM_SHAPETYPE_RECTANGLE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(gets("rect"))
		},
		{
			3, IDM_SHAPETYPE_ELLIPSE, TBSTATE_ENABLED, buttonStyles,{0}, 0,
			reinterpret_cast<INT_PTR>(gets("ellipse"))
		}
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, WPARAM(sizeof(TBBUTTON)), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, WPARAM(numButtons), LPARAM(&tbButtons));

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}

HWND MainWindow::createStatusBar(int sbid, int cParts)
{
	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create the status bar.
	HWND hwndStatus = CreateWindowEx(
		0, // no extended styles
		STATUSCLASSNAME, // name of status bar class
		strings::instance()->format("status_fmt", strings::instance()->get("point")), // no text when first created
		SBARS_SIZEGRIP | // includes a sizing grip
		WS_CHILD | WS_VISIBLE, // creates a visible child window
		0, 0, 0, 0, // ignores size and position
		hWnd, // handle to parent window
		HMENU(sbid), // child window identifier
		hInst, // handle to application instance
		NULL); // no window creation data

	// Allocate an array for holding the right edge coordinates.
	auto hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	auto sb_paParts = static_cast<PINT>(LocalLock(hloc));

	RECT rcClient;
	GetClientRect(hWnd, &rcClient);


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

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);
	return hwndStatus;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
			case IDM_SHAPETYPE_POINT:
				controller->SetShapeType(ST_POINT);
				controller->SetInputMethod(IM_CORNERCORNER);
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
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
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
	case WM_DESTROY:
		controller = NULL;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (hTool) SendMessage(hTool, WM_SIZE, 0, 0);
		if (hStatus) SendMessage(hStatus, WM_SIZE, 0, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK MainWindow::AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return INT_PTR(TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return INT_PTR(TRUE);
		}
		break;
	}
	return INT_PTR(FALSE);
}


LPCWSTR MainWindow::gets(TKEY key)
{
	return strings::instance()->get(key);
}

void MainWindow::formatWindowText(HWND hWnd, TKEY str_key)
{
	::SetWindowTextW(hWnd, strings::instance()->format("status_fmt", gets(str_key)));
}

MainWindow::~MainWindow()
{
}
