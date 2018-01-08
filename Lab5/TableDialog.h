#pragma once
#include "stdafx.h"
#include "CustomTableControl.h"

INT_PTR CALLBACK	TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND				CreateTableDialog(HINSTANCE hInst, HWND hwndParent);

void				TblDlgSetData(HWND hDlg, CustomTableData* data);
CustomTableData*	TblDlgGetData(HWND hDlg);
void				TblDlgNotifyDataChanged(HWND hDlg);