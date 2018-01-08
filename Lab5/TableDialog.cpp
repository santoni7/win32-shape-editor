#include "stdafx.h"
#include "TableDialog.h"
#include "CustomTableControl.h"
#include "resource1.h"
#include "CustomEditTable.h"

static HINSTANCE hInst;
static CustomTableData* customData;
static SelectionChangedListener listener;
HWND CreateTableDialog(HINSTANCE hInstance, HWND hwndParent)
{
	hInst = hInstance;
	HWND hDlg = CreateDialog(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		hwndParent, (DLGPROC)TableDlgProc);
	return hDlg;
}

void TblDlgSetData(HWND hDlg, CustomTableData * data)
{
	CustomTable* cTable = (CustomTable*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (cTable) {
		cTable->GetData()->hwnd = cTable->hwnd();
		cTable->SetData(data);
		cTable->Update();
	}
	customData = data;
}
void TblDlgNotifyDataChanged(HWND hDlg)
{
	CustomTable* cTable = (CustomTable*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (cTable) { 
		cTable->SetData(customData);
		cTable->Update(); 
	}
	InvalidateRect(cTable->hwnd(), NULL, TRUE);
}

CustomTableData* TblDlgGetData(HWND hDlg)
{
	CustomTable* cTable = (CustomTable*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	if (cTable) return cTable->GetData();
	return nullptr;
}

#define _MARGIN_ 5
INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	CustomTable* cTable = (CustomTable*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	switch (message)
	{
	case WM_INITDIALOG:		
	{
		DWORD style = XXS_DOUBLEBUFFER | XXS_WEIGHTSIZING | XXS_BACKGROUND_AB | XXS_ENABLE_ROWSELECT | XXS_HEADER_ROW | XXS_HEADER_COLUMN | XXS_INIT_EMPTY_DATA;
		auto cTable = new CustomTable(hDlg, hInst, 1239, style, 5, 1);
		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)cTable);

		RECT rc; GetClientRect(hDlg, &rc);
		int h = rc.bottom;
		int w = rc.right;
		cTable->SetPos(_MARGIN_, _MARGIN_, w - 2 * _MARGIN_, h - 2 * _MARGIN_);
		cTable->Focus();
		cTable->Update();
		return INT_PTR(TRUE);
	}
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;
	case WM_SIZE: {
		int h = HIWORD(lParam);
		int w = LOWORD(lParam);
		cTable->SetPos(_MARGIN_, _MARGIN_, w - 2 * _MARGIN_, h - 2 * _MARGIN_);
		return 0;
	}
	case WM_KEYDOWN:
		if (!cTable) return 0;
		cTable->Focus();
		PostMessage(cTable->hwnd(), message, wParam, lParam);
		return 0;
	case WM_LBUTTONDOWN:
		/*SetFocus(hCustomTable);*/
		return 0;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			auto pData = cTable->GetData();
			pData->onSelectionChanged(pData, -1, pData->nSelected);
			DestroyWindow(hDlg);
			return INT_PTR(TRUE);
		}
		break;
	}
	return INT_PTR(FALSE);
}