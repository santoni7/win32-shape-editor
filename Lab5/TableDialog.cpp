#include "stdafx.h"
#include "TableDialog.h"
#include "CustomTable.h"
#include "resource1.h"

static HWND hCustomTable;
static CustomTableData* customData;
HWND CreateTableDialog(HINSTANCE hInst, HWND hwndParent)
{
	return CreateDialog(hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		hwndParent, (DLGPROC)TableDlgProc);
}

void TblDlgSetData(CustomTableData * data)
{
	customData = data;
	if (hCustomTable) SetCustomTableData(hCustomTable, data);

}

INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:		
		hCustomTable = GetDlgItem(hDlg, IDC_CUSTOMTABLE);
		if(customData)
			SetCustomTableData(hCustomTable, customData);
		return INT_PTR(TRUE);
	case WM_VSCROLL:
		return INT_PTR(TRUE);
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