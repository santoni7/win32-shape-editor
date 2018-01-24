#pragma once
#include "stdafx.h"
#include "XGlobal.h"
class XGdiContext : public XContext
{
public:
	HDC hdc;
	HWND hWnd;
	HDC CreateHDC() {
		if (hWnd)
			return GetDC(hWnd);
		return 0;
	}
	XGdiContext CreateDCContext() {
		XGdiContext ctx(GetDC(hWnd));
		ctx.hdc = GetDC(hWnd);
		ctx.hWnd = hWnd;
		return ctx;
	}
	void ReleaseHDC() {
		if (hWnd && hdc) ReleaseDC(hWnd, hdc);
	}
	XGdiContext() = default;
	XGdiContext(HDC hDC) : hdc(hDC) { }
	XGdiContext(HWND hWnd) : hWnd(hWnd) { }
	XGdiContext(HWND hWnd, HDC hDC) : hWnd(hWnd), hdc(hDC) { }
};
class XGdiColor : public XColor
{
public:
	COLORREF cr;
	bool transparent;
	XGdiColor(COLORREF col, bool transparent = false) : cr(col), transparent(transparent) { }
	HBRUSH CreateBrush() {
		return (HBRUSH)(transparent ? (HBRUSH)GetStockObject(NULL_BRUSH) : CreateSolidBrush(cr));
	}
};