#include "stdafx.h"
#include "CustomTable.h"

static void
CustomDoubleBuffer(HWND hwnd, PAINTSTRUCT* pPaintStruct);
static struct {
	HBRUSH hBrush, hBrushOld;
	HPEN hPenOld, hPen;
	COLORREF textCol, textColOld;
} render_info;

static void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) 
{
	::MoveToEx(hdc, x1, y1, nullptr);
	::LineTo(hdc, x2, y2);
}

static void 
BeforeRender(HDC hdc, bool fill, COLORREF fillcol, COLORREF outlinecol, COLORREF textcol) 
{
	render_info.hBrush = (HBRUSH)(fill
		? ::CreateSolidBrush(fillcol)
		: ::GetStockObject(NULL_BRUSH));
	render_info.hBrushOld = (HBRUSH)::SelectObject(hdc, render_info.hBrush);

	render_info.hPen = ::CreatePen(PS_SOLID, 1, outlinecol);
	render_info.hPenOld = (HPEN)::SelectObject(hdc, render_info.hPen);
	render_info.textColOld = GetTextColor(hdc);
	render_info.textCol = textcol;
	SetTextColor(hdc, textcol);
}

static void 
AfterRender(HDC hdc) {
	::SelectObject(hdc, render_info.hBrushOld);
	::DeleteObject(render_info.hBrush);
	::SelectObject(hdc, render_info.hPenOld);
	::DeleteObject(render_info.hPen);
	::SetTextColor(hdc, render_info.textColOld);
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
	SetBkColor(hdc, RGB(0, 244, 244));
	SetBkMode(hdc, TRANSPARENT);
	int top = 0;
	for (int i = 0; i < data->cntRow; ++i) {
		CustomTableRow *row = data->rows.at(i);
		int left = 0;
		int bottom = top + row->height;
		if (row->selected){
			BeforeRender(hdc, !row->transparent, data->bkgcol_sel, NULL, data->txtcol_sel);
			Rectangle(hdc, left, top, data->width+1, bottom+1);
		}
		else {
			BeforeRender(hdc, !row->transparent, data->bkgcol, NULL, data->txtcol);
		}
		for (int j = 0; j < data->cntCol; ++j) {
			CustomTableCell *cell = row->cells.at(j);
			int right = left + data->colW[j];
			DrawLine(hdc, left, top, left, bottom);
			DrawLine(hdc, right, top, right, bottom);
			
			r.left = left;
			r.right = right;
			r.bottom = bottom;
			r.top = top;
			DrawText(hdc, cell->text, -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			left = right;
		}
		DrawLine(hdc, 0, top, data->width, top);
		DrawLine(hdc, 0, bottom, data->width, bottom);
		top = bottom;
		AfterRender(hdc);
	}

	EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CustomTableData* pData = GetCustomTableData(hwnd);// (CustomTableData*)GetWindowLongPtr(hwnd, 0);
	switch (uMsg) {
	case WM_VSCROLL:

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	case WM_NCCREATE:
		// if already allocated by outside, return
		if (pData) return TRUE;
		pData = new CustomTableData;
		if (pData == NULL)
			return FALSE;
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);
		pData->cntCol = 2;
		pData->cntRow = 2;
		pData->colW = new int[5]{ 150 };
		pData->width = 50 * 5;
		pData->height = 80;
		//Dummy data
		pData->rows = std::vector<CustomTableRow*>{
			new CustomTableRow(
				std::vector<CustomTableCell*>
					{
						new CustomTableCell{ L"test1" },
						new CustomTableCell{ L"test2" }
					}, 30),
			new CustomTableRow(
				std::vector<CustomTableCell*>
					{
						new CustomTableCell{ L"test3" },
						new CustomTableCell{ L"test4" }
					}, 30),
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

static void
CustomDoubleBuffer(HWND hwnd, PAINTSTRUCT* pPaintStruct)
{
	int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
	int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
	HDC hMemDC;
	HBITMAP hBmp;
	HBITMAP hOldBmp;
	POINT ptOldOrigin;

	// Create new bitmap-back device context, large as the dirty rectangle.
	hMemDC = CreateCompatibleDC(pPaintStruct->hdc);
	hBmp = CreateCompatibleBitmap(pPaintStruct->hdc, cx, cy);
	hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

	// Do the painting into the memory bitmap.
	OffsetViewportOrgEx(hMemDC, -(pPaintStruct->rcPaint.left),
		-(pPaintStruct->rcPaint.top), &ptOldOrigin);
	/////////CustomPaint(hwnd, hMemDC, &pPaintStruct->rcPaint, TRUE);
	SetViewportOrgEx(hMemDC, ptOldOrigin.x, ptOldOrigin.y, NULL);

	// Blit the bitmap into the screen. This is really fast operation and although
	// the CustomPaint() can be complex and slow there will be no flicker any more.
	BitBlt(pPaintStruct->hdc, pPaintStruct->rcPaint.left, pPaintStruct->rcPaint.top,
		cx, cy, hMemDC, 0, 0, SRCCOPY);

	// Clean up.
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
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

CustomTableData* GetCustomTableData(HWND hwnd)
{
	return (CustomTableData*)GetWindowLongPtr(hwnd, 0);
}

void SetCustomTableData(HWND hCtl, CustomTableData* data) 
{
	SetWindowLongPtr(hCtl, 0, (LONG_PTR)data);
}
