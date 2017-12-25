#pragma once
#include <tchar.h>
#include <windows.h>
#include <vector>

/* Window class */
#define CUSTOMTABLE   L"com.santoni7.CustomTable"

/* Register/unregister the window class */
void CustomRegister(void);
void CustomUnregister(void);

/*	Control specific data */
typedef struct CustomTableData_type CustomTableData;
typedef struct CustomTableCell_type CustomTableCell;
typedef struct CustomTableRow_type CustomTableRow;

CustomTableData* GetCustomTableData(HWND hCtl);
void SetCustomTableData(HWND hCtl, CustomTableData* data);

struct CustomTableData_type {
	int cntCol, cntRow;
	int width, height;
	int scrollX, scrollY;
	int *colW;// , *rowH;
	COLORREF bkgcol = RGB(255, 255, 255);
	COLORREF txtcol = RGB(0, 0, 0);
	COLORREF bkgcol_sel = RGB(100, 100, 244);
	COLORREF txtcol_sel = RGB(255, 255, 255);
	std::vector<CustomTableRow*> rows;
};
struct CustomTableRow_type 
{
	std::vector<CustomTableCell*> cells;
	int height;
	bool selected = false;
	bool transparent = false;
	CustomTableRow_type() {}
	CustomTableRow_type(std::vector<CustomTableCell*> cells, int height) : cells(cells), height(height) {};
};
struct CustomTableCell_type {
	LPCWSTR text;
	CustomTableCell_type(LPCWSTR text) : text(text) { }
	CustomTableCell_type() {}
};