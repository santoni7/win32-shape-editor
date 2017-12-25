#pragma once
#include "stdafx.h"
#include "CustomTable.h"
INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND CreateTableDialog(HINSTANCE hInst, HWND hwndParent);
void TblDlgSetData(CustomTableData* data);
