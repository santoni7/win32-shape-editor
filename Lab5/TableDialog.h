#pragma once
#include "stdafx.h"

INT_PTR CALLBACK TableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HWND CreateTableDialog(HINSTANCE hInst, HWND hwndParent);

