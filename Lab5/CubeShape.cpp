#include "stdafx.h"
#include "CubeShape.h"
#include <cmath>
#include <algorithm>
//#define MAX(a, b) ((a)>(b)?(a):(b))
#define ABS(a) (((a)<0)?(-a):(a))

CubeShape::CubeShape(){ }

Shape* CubeShape::copy() {
	return new CubeShape(*this);
}

CubeShape::CubeShape(MPoint p1, MPoint p2) 
{
	SetPoints(p1, p2);
}
void CubeShape::SetPoints(MPoint p1, MPoint p2) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int width = (ABS(dx) > ABS(dy)) ? dx : dy;
	this->width = ABS(width);
	this->p1 = p1;
	this->p2 = MPoint(p1.x + width, p1.y + width);

	if (this->p1.x > this->p2.x) std::swap(this->p1, this->p2);
}
void CubeShape::RenderSimple(HDC hdc) {
	int delta = width / 4;// (int)(2 * sqrt(width));
	::Rectangle(hdc, p1.x, p1.y, p2.x +1 , p2.y +1);
	::Rectangle(hdc, p1.x + delta, p1.y - delta, p2.x + delta +1 , p2.y - delta +1 );
	renderLine(hdc, p1, MPoint(p1.x + delta, p1.y - delta));
	renderLine(hdc, MPoint(p1.x + width, p1.y), MPoint(p1.x + width + delta, p1.y - delta));
	renderLine(hdc, MPoint(p1.x, p1.y + width), MPoint(p1.x + delta, p1.y + width - delta));
	renderLine(hdc, p2, MPoint(p2.x + delta, p2.y - delta));
}
void CubeShape::renderLine(HDC hdc, MPoint p1, MPoint p2) {
	MoveToEx(hdc, p1.x, p1.y, nullptr);
	LineTo(hdc, p2.x, p2.y);
}
CubeShape::~CubeShape()
{
}
