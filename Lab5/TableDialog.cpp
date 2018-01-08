#include "stdafx.h"
#include "TableDialog.h"
#include "CustomTableControl.h"
#include "resource1.h"
#include "CustomTableClass.h"

//static HWND hCustomTable;
CustomTable* cTable;
static CustomTableData* customData;
static SelectionChangedListener listener;
HWND CreateTableDialog(HINSTANCE hInst, HWND hwndParent)
{
	return CreateDialog(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		hwndParent, (DLGPROC)TableDlgProc);
}

void TblDlgSetData(CustomTableData * data)
{
	if (cTable) {
		cTable->SetData(data);
	}
	customData = data;
}
void TblDlgNotifyDataChanged()
{
	if (cTable) cTable->Update();
}


CustomTableData* TblDlgGetData()
{
	if (cTable) return cTable->GetData();
	return nullptr;
}

INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:		
	{
		DWORD style = XXS_ALLSTYLES;
		cTable = new CustomTable(hDlg, 0, 1234, style);// CreateWindowEx(0, CUSTOMTABLE_CLASS, L"", WS_CHILD | WS_VISIBLE, 0, 0, 400, 400, hDlg, 0, 0, style);// GetDlgItem(hDlg, IDC_CUSTOMTABLE);
		if(customData) cTable->SetData(customData);
		cTable->Focus();
		return INT_PTR(TRUE);
	}
	/*case WM_NOTIFY:
		PostMessage(hCustomTable, WM_NOTIFY, 0, 0);
		return INT_PTR(TRUE);*/

	//case WM_GETDLGCODE:
	//	return DLGC_WANTALLKEYS;
	case WM_SIZE: {
		int h = HIWORD(lParam);
		int w = LOWORD(lParam);
#define _MARGIN_ 5
		cTable->SetPos(_MARGIN_, _MARGIN_, w - 2 * _MARGIN_, h - 2 * _MARGIN_);
		//SetWindowPos(hCustomTable, nullptr, _MARGIN_, _MARGIN_, w- 2*_MARGIN_, h- 2*_MARGIN_, SWP_SHOWWINDOW | SWP_NOZORDER);
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
			//EndDialog(hDlg, LOWORD(wParam));
			DestroyWindow(hDlg);
			//hwndTableDlg = NULL;
			return INT_PTR(TRUE);
		}
		break;
	}
	return INT_PTR(FALSE);
}