#include "stdafx.h"
#include "CustomTable.h"
typedef struct CustomTableData_type CustomTableData;
struct CustomTableData_type {
	int cntCol, cntRow;
	int width, height;
	int scrollX, scrollY;
	int *colW, *rowH;
	LPCWSTR** data;
};

static void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) 
{
	::MoveToEx(hdc, x1, y1, nullptr);
	::LineTo(hdc, x2, y2);
}

static void
CustomPaint(HWND hwnd, CustomTableData* data)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	GetClientRect(hwnd, &rect);
	RECT r(rect);
	hdc = BeginPaint(hwnd, &ps);
	SetTextColor(hdc, RGB(0, 0, 0));
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(244, 244, 244));
	int top = 0;
	for (int i = 0; i < data->cntRow; ++i) {
		int left = 0;
		int bottom = top + data->rowH[i];
		for (int j = 0; j < data->cntCol; ++j) {
			int right = left + data->colW[j];
			DrawLine(hdc, left, top, left, bottom);
			DrawLine(hdc, right, top, right, bottom);
			
			r.left = left;
			r.right = right;
			r.bottom = bottom;
			r.top = top;
			DrawText(hdc, data->data[i][j], -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			left = right;

		}
		DrawLine(hdc, 0, top, data->width, top);
		DrawLine(hdc, 0, bottom, data->width, bottom);
		top = bottom;
	}

	EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CustomTableData* pData = (CustomTableData*)GetWindowLongPtr(hwnd, 0);
	switch (uMsg) {
	case WM_VSCROLL:

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	case WM_NCCREATE:
		// Allocate, setupo and remember the control data:
		pData = new CustomTableData;// malloc(sizeof(CustomTableData));
		if (pData == NULL)
			return FALSE;
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);
		pData->cntCol = 5;
		pData->cntRow = 3;
		pData->colW = new int[5]{ 50,50,50,50,50 };
		pData->rowH = new int[3]{ 30,20,30 };
		pData->width = 50 * 5;
		pData->height = 80;
		pData->data = new LPCWSTR*[3]{
			new LPCWSTR[5]{ L"test1", L"test", L"test", L"test", L"test" },
			new LPCWSTR[5]{ L"test2", L"test", L"test", L"test", L"test" },
			new LPCWSTR[5]{ L"test3", L"test", L"test", L"test", L"test" }
		};
		return TRUE;
	case WM_PAINT:
		CustomPaint(hwnd, pData);
		return 0;
	case WM_NCDESTROY:
		if (pData)
			delete pData;
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CustomRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CustomProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = CUSTOMTABLE;
	wc.cbWndExtra = sizeof(CustomTableData);
	RegisterClass(&wc);
}

void
CustomUnregister(void)
{
	UnregisterClass(CUSTOMTABLE, NULL);
}
