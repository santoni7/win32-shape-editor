#pragma once
#include "stdafx.h"
#include "Helpers.h"

 class Shape {
protected:
	MPoint p1, p2;
	COLORREF outlineCol;
	COLORREF fillCol;
	bool shouldFill;
	struct {
		HBRUSH hBrush, hBrushOld;
		HPEN hPenOld, hPen;
	} render_info;
	virtual void beforeRender(HDC hdc);
	virtual void afterRender(HDC hdc);
public:
	Shape(const Shape& s) : p1(s.p1), p2(s.p2),
		outlineCol(s.outlineCol), fillCol(s.fillCol), shouldFill(s.shouldFill), render_info(s.render_info) { }
	Shape(){}
	Shape(MPoint p1, MPoint p2)
		: p1(p1), p2(p2) { }
	virtual Shape* copy() = 0;
	virtual void SetPoints(MPoint p1, MPoint p2);
	void SetPoints(int x1, int y1, int x2, int y2);
	virtual void SetOutlineColor(COLORREF color);
	virtual void SetFillColor(COLORREF color, bool shouldFill);

	virtual void Render(HDC hdc);
	virtual void RenderSimple(HDC hdc) = 0;
	//virtual void RenderCustom(HDC hdc);
	virtual ~Shape() { };
};

