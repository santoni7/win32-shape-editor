#include "stdafx.h"
#include "Shapes.h"

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

// PointShape
void PointShape::Render(HDC hdc) {
	::SetPixel(hdc, this->p1.x, this->p1.y, this->outlineCol);
	
}
PointShape::~PointShape()
{
}

//	LineShape
void LineShape::Render(HDC hdc)
{
	::SetPixel(hdc, this->p1.x, this->p2.x, this->outlineCol);
	::MoveToEx(hdc, p1.x, p1.y, nullptr);
	::LineTo(hdc, p2.x, p2.y);
}

LineShape::~LineShape()
{
}

//	RectShape
void RectShape::Render(HDC hdc) {
	HBRUSH hBrush, hBrushOld;
	HPEN hPenOld, hPen;

	hBrush = HBRUSH(shouldFill
		                ? ::CreateSolidBrush(fillCol)
		                : ::GetStockObject(NULL_BRUSH));
	hBrushOld = HBRUSH(::SelectObject(hdc, hBrush));

	hPen = ::CreatePen(PS_SOLID, 1, outlineCol);
	hPenOld = (HPEN)::SelectObject(hdc, hPen);

	::Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
	::SelectObject(hdc, hBrushOld); 
	::SelectObject(hdc, hPenOld);
	::DeleteObject(hBrush);
	::DeleteObject(hPen);
}
RectShape::~RectShape()
{ }


// EllipseShape
void EllipseShape::Render(HDC hdc) {
	HBRUSH hBrush, hBrushOld;
	HPEN hPenOld, hPen;

	hBrush = (HBRUSH)(shouldFill
		? ::CreateSolidBrush(fillCol)
		: ::GetStockObject(NULL_BRUSH));
	hBrushOld = (HBRUSH)::SelectObject(hdc, hBrush);

	hPen = ::CreatePen(PS_SOLID, 1, outlineCol);
	hPenOld = (HPEN)::SelectObject(hdc, hPen);

	::Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);


	::SelectObject(hdc, hBrushOld);
	::DeleteObject(hBrush);
	::SelectObject(hdc, hPenOld);
	::DeleteObject(hPen);
}
EllipseShape::~EllipseShape()
{ }



///// Factory static method
//Shape* Shape::CreateShape(MPoint p1, MPoint p2, ShapeType shapeType, InputMethod inputMethod)
//{
//	switch (shapeType) {
//	case ST_POINT:
//		return new PointShape(p1);
//	case ST_RECTANGLE:
//		return new RectShape(p1, p2);
//	case ST_ELLIPSE:
//		return new EllipseShape(p1, p2);
//	default:
//		return new PointShape(p1);
//	}
//}
