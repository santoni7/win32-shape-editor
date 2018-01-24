#include "stdafx.h"
#include "MainWindow.h"
#include "CustomTableControl.h"
#include "resource.h"
#include "strings.h"
#include "TableDialog.h"
#include "DrawController.h"
#include "XGlobal.h"
#include "XGdiGlobal.h"
#include "XDrawController.h"
#include "XImplStorage.h"
//status bar parts
#define SB_PARTS 2

static HINSTANCE hInst;
static HWND hwndMain;
static HWND hStatus, hTool;

extern HWND hwndTableDlg;
static HWND MW_createToolbar();
static HWND MW_createStatusBar(int sbid, int cParts = SB_PARTS);
static void MW_updateStatusBarSize(int cParts = SB_PARTS);

static INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static void MW_formatWindowText(HWND hWnd, TKEY str_key);
static void MW_formatShapeCountText(int count);
static LPCWSTR gets(TKEY key);

XImplStorage* impStorage;

static XGdiDrawController *controller;
static XGdiColor *gGreenColor = new XGdiColor(RGB(50, 200, 50));
static XGdiColor *gRedColor = new XGdiColor(RGB(200, 50, 50));
static XGdiColor *gBlackColor = new XGdiColor(RGB(0, 0, 0));
static XGdiColor *gTransparentColor = new XGdiColor(RGB(0, 0, 0), true);
ATOM MainRegisterWndClass(HINSTANCE hInstance)
{
	hInst = hInstance;
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
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

BOOL MainInit(int nCmdShow)
{
	hwndMain = CreateWindowW(gets("wnd_class_name"), gets("app_title"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr,
		hInst, nullptr);
	if (!hwndMain)
	{
		return FALSE;
	}
	XGdiContext ctx(hwndMain);
	XImplStorage::inst()->AddImpl<XPathShapeImpl>(new XGdiPathShapeImpl());
	XImplStorage::inst()->AddImpl<XPointImpl>(new XGdiPointShapeImpl());
	XBasicShapeFactory factory;

	controller = new XGdiDrawController(ctx);
	controller->Start(factory.CreatePath(std::vector<MPoint>({MPoint(100,100), MPoint(200, 200)}), true), IM_CORNERCORNER, gBlackColor, gBlackColor);

	hTool = MW_createToolbar();

	const int SB_ID = 1001;
	hStatus = MW_createStatusBar(SB_ID);

	ShowWindow(hwndMain, nCmdShow);
	UpdateWindow(hwndMain);

	MW_formatShapeCountText(controller->GetShapesCount());
	return TRUE;
}

static CustomTableRow* BuildTableRow(Shape* sh)
{
	strings* str = strings::instance();
	CustomTableRow* row = new CustomTableRow;
	row->cells.push_back(new CustomTableCell(str->get(sh->SimpleName())));
	row->cells.push_back(new CustomTableCell(str->format("numfmt", sh->p1.x)));
	row->cells.push_back(new CustomTableCell(str->format("numfmt", sh->p1.y)));
	row->cells.push_back(new CustomTableCell(str->format("numfmt", sh->p2.x)));
	row->cells.push_back(new CustomTableCell(str->format("numfmt", sh->p2.y)));
	return row;
}


/* Called every time selected row changes */
void CALLBACK OnSelChanged(CustomTableData* pData, int nSelected, int nPrevSelected)
{
	InvalidateRect(hwndMain, NULL, TRUE);
}
static void UpdateTableData(CustomTableData* pData, std::vector<Shape*>& shapes, int n)
{
	pData->rows.clear();
	pData->nCols = 5;
	pData->col_headings = std::vector<LPCWSTR>({ L"�����", L"X1", L"X2", L"X3", L"X4" });
	pData->col_wg[0] = 1.5f;
	pData->onSelectionChanged = &OnSelChanged;
	for (int i = 0; i<n; ++i)
	{
		Shape* sh = shapes.at(i);
		CustomTableRow* row = BuildTableRow(sh);
		pData->rows.push_back(row);
	}
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XGdiContext ctx(hWnd);
	switch (message)
	{
	case WM_CREATE:
		//hCustomTable = CreateWindow(CUSTOMTABLE, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 300, 300, hWnd, HMENU(CUSTOMID), hInst, NULL);
		break;
	case WM_LBUTTONDOWN:
		controller->OnMouseDown(ctx);
		break;
	case WM_LBUTTONUP:
		controller->OnMouseUp(ctx);
		MW_formatShapeCountText(controller->GetShapesCount());
		//if (IsWindow(hwndTableDlg)) {
		//	UpdateTableData(TblDlgGetData(hwndTableDlg), controller->GetShapes(), controller->GetShapesCount());
		//	TblDlgNotifyDataChanged(hwndTableDlg);
		//}
		break;
	case WM_MOUSEMOVE:
		controller->OnMouseMove(ctx);
		break;
	case WM_PAINT:
		controller->OnPaint(ctx);
		break;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// ��������� ����� � ����:
			XBasicShapeFactory factory;
			switch (wmId)
			{
			case IDM_SHAPETYPE_POINT:

				//controller->Start(new PointShape, IM_CORNERCORNER, RGB(0,0,0), RGB(0,0,0), true);
				//XBasicShapeFactory()
				//controller->Start(factory.CreatePoint(), IM_CORNERCORNER, gBlackColor, gBlackColor);
				controller->Start(factory.CreatePath(std::vector<MPoint>(), true), IM_CORNERCORNER, gBlackColor, gBlackColor);

				MW_formatWindowText(hStatus, "point");
				break;
			case IDM_SHAPETYPE_LINE:
				controller->Start(factory.CreateLine(), IM_CORNERCORNER, gBlackColor, gBlackColor);
				MW_formatWindowText(hStatus, "line");
				break;
			case IDM_SHAPETYPE_RECTANGLE:
				controller->Start(factory.CreateLineOO(), IM_CENTERCORNER, gBlackColor, gTransparentColor);
				MW_formatWindowText(hStatus, "rect");
				break;
			case IDM_SHAPETYPE_ELLIPSE:
				controller->Start(factory.CreateEllipse(), IM_CORNERCORNER, gBlackColor, gTransparentColor);
				MW_formatWindowText(hStatus, "ellipse");
				break;
			//case IDM_SHAPETYPE_LINEOO:
			//	controller->Start(new LineOOShape, IM_CORNERCORNER, RGB(0, 0, 0), RGB(30, 165, 160), true);
			//	MW_formatWindowText(hStatus, "lineoo");
			//	break;
			//case IDM_SHAPETYPE_CUBE:
			//	controller->Start(new CubeShape, IM_CORNERCORNER, RGB(0, 0, 0), NULL, false);
			//	MW_formatWindowText(hStatus, "cube");
			//	break;
			//case IDM_EDITTABLE:
			//	{
			//		if (!IsWindow(hwndTableDlg)) {
			//			hwndTableDlg = CreateTableDialog(hInst, hWnd);
			//			ShowWindow(hwndTableDlg, SW_SHOW);
			//		}
			//		UpdateTableData(TblDlgGetData(hwndTableDlg), controller->GetShapes(), controller->GetShapesCount());
			//	}
			//	break;
			//case IDM_SAVE:
			//{
			//	DataWriter dw(hWnd);
			//	std::vector<Shape*> &sh = controller->GetShapes();
			//	if (dw.OpenFileDlg()) {
			//		for (int i = 0; i < controller->GetShapesCount(); ++i)
			//		{
			//			dw.WriteNext(sh[i]);
			//		}
			//		dw.CloseFile();
			//	}
			//}
			//	break;
			//case IDM_OPEN:
			//{
			//	DataReader dr(hWnd);
			//	if (dr.OpenFileDlg()) {
			//		Shape* sh;
			//		std::vector<Shape*> new_shapes;
			//		while ((sh = dr.ReadNext())) {
			//			new_shapes.push_back(sh);
			//		}
			//		dr.CloseFile();
			//		controller->SetShapes(new_shapes, new_shapes.size());
			//		controller->Start(new PointShape);
			//	}
			//	InvalidateRect(hWnd, NULL, TRUE);
			//}
			//	break;
			//case IDM_CLEAR:
			//	controller->Clear();
			//	InvalidateRect(hWnd, NULL, TRUE);
			//	break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDM_UNDO:
				controller->Undo(ctx);
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
		MW_updateStatusBarSize();
		if (hTool) SendMessage(hTool, WM_SIZE, 0, 0);
		if (hStatus) SendMessage(hStatus, WM_SIZE, 0, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

LPCWSTR gets(TKEY key)
{
	return strings::instance()->get(key);
}

void MW_formatWindowText(HWND hWnd, TKEY str_key)
{
	::SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)strings::instance()->format("status_fmt", gets(str_key)));
}

void MW_formatShapeCountText(int count) 
{
	::SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)strings::instance()->format("shape_count_fmt", count));
}


HWND MW_createToolbar()
{
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 6;
	const int bitmapSize = 32;

	const DWORD buttonStyles = TBSTYLE_BUTTON;// BTNS_AUTOSIZE;

											  // Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS, 0, 0, 0, 0,
		hwndMain, NULL, hInst, NULL);

	if (hWndToolbar == nullptr)
		return nullptr;


	COLORREF mask = RGB(255, 255, 255);
	HIMAGELIST hImageList = ImageList_LoadBitmap(hInst, MAKEINTRESOURCEW(IDB_PT), bitmapSize, 0, mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LINE)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RECT)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELLIPSE)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LINEOO)), mask);
	ImageList_AddMasked(hImageList, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CUBE)), mask);


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
			0, IDM_SHAPETYPE_POINT, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("point"))
		},
		{
			1, IDM_SHAPETYPE_LINE, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("line"))
		},
		{
			2, IDM_SHAPETYPE_RECTANGLE, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("rect"))
		},
		{
			3, IDM_SHAPETYPE_ELLIPSE, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("ellipse"))
		},
		{
			4, IDM_SHAPETYPE_LINEOO, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("lineoo"))
		},
		{
			5, IDM_SHAPETYPE_CUBE, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0,
			reinterpret_cast<INT_PTR>(gets("cube"))
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

HWND MW_createStatusBar(int sbid, int cParts)
{
	// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Create the status bar.
	HWND hwndStatus = CreateWindowEx(0, STATUSCLASSNAME,
		strings::instance()->format("status_fmt", strings::instance()->get("point")),
		SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0,
		hwndMain, HMENU(sbid), hInst, NULL);
	// Allocate an array for holding the right edge coordinates.
	auto hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	auto sb_paParts = static_cast<PINT>(LocalLock(hloc));

	RECT rcClient;
	GetClientRect(hwndMain, &rcClient);


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

void MW_updateStatusBarSize(int cParts)
{
	if (!hwndMain) return;
	RECT rcClient;
	GetClientRect(hwndMain, &rcClient);


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
	SendMessage(hStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)
		sb_paParts);


}
