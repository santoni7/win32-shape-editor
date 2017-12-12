#include "stdafx.h"
#include "shape.h"
#include "basic_shapes.h"

// PointShape
void PointShape::RenderSimple(HDC hdc) {
#ifdef SH_POINT_WIDTH
	const int w = SH_POINT_WIDTH;
	::Ellipse(hdc, p1.x - w / 2, p1.y - w / 2, p1.x + w / 2, p1.y + w / 2);
#else 
	::SetPixel(hdc, p1.x, p1.y, fillCol);
#endif
}
Shape* PointShape::copy() {
	return new PointShape(*this);
}
PointShape::~PointShape()
{
}

//	LineShape
void LineShape::RenderSimple(HDC hdc)
{
	::MoveToEx(hdc, p1.x, p1.y, nullptr);
	::LineTo(hdc, p2.x, p2.y);
}
Shape* LineShape::copy() {
	return new LineShape(*this);
}
LineShape::~LineShape()
{
}

//	RectShape
void RectShape::RenderSimple(HDC hdc) {
	::Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
}
Shape* RectShape::copy() {
	return new RectShape(*this);
}
RectShape::~RectShape()
{ }


// EllipseShape
void EllipseShape::RenderSimple(HDC hdc) {
	::Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);
}
Shape* EllipseShape::copy() {
	return new EllipseShape(*this);
}
EllipseShape::~EllipseShape()
{ }

