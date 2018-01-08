#include "stdafx.h"
#include "shape.h"
#include "DrawController.h"


// Shape
void Shape::SetPoints(MPoint p1, MPoint p2) {
	this->p1 = p1;
	this->p2 = p2;
}
void Shape::SetPoints(int x1, int y1, int x2, int y2) {
	this->p1 = MPoint(x1, y1);
	this->p2 = MPoint(x2, y2);
}
void Shape::SetOutlineColor(COLORREF color)
{
	this->outlineCol = color;
}

void Shape::SetFillColor(COLORREF color, bool shouldFill)
{
	this->fillCol = color;
	this->shouldFill = shouldFill;
}

Shape::render_data Shape::GetRenderData()
{
	Shape::render_data data;
	data.outlineCol = this->outlineCol;
	data.fillCol = this->fillCol;
	data.shouldFill = this->shouldFill;
	return data;
}

void Shape::beforeRender(HDC hdc) {
	render_info.hBrush = (HBRUSH)(shouldFill
		? ::CreateSolidBrush(fillCol)
		: ::GetStockObject(NULL_BRUSH));
	render_info.hBrushOld = (HBRUSH)::SelectObject(hdc, render_info.hBrush);

	render_info.hPen = ::CreatePen(PS_SOLID, 1, outlineCol);
	render_info.hPenOld = (HPEN)::SelectObject(hdc, render_info.hPen);
}

void Shape::afterRender(HDC hdc) {
	::SelectObject(hdc, render_info.hBrushOld);
	::DeleteObject(render_info.hBrush);
	::SelectObject(hdc, render_info.hPenOld);
	::DeleteObject(render_info.hPen);
}

void Shape::Render(HDC hdc) {
	beforeRender(hdc);
	RenderSimple(hdc);
	afterRender(hdc);
}
