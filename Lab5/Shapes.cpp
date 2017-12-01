#include "stdafx.h"
#include "Shapes.h"
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

// PointShape
void PointShape::Render(HDC hdc) {
#ifdef SH_POINT_WIDTH
	const int w = SH_POINT_WIDTH;
	HBRUSH hBrush = static_cast<HBRUSH>(::CreateSolidBrush(RGB(0,0,0)));
	HBRUSH hBrushOld = static_cast<HBRUSH>(::SelectObject(hdc, hBrush));

	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hPenOld = static_cast<HPEN>(::SelectObject(hdc, hPen));

	::Ellipse(hdc, p1.x-w/2, p1.y-w/2, p1.x + w/2, p1.y + w/2);
	
	::SelectObject(hdc, hBrushOld);
	::DeleteObject(hBrush);
	::SelectObject(hdc, hPenOld);
	::DeleteObject(hPen);
#else 
	::SetPixel(hdc, p1.x, p1.y, fillCol);
#endif
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


LineOOShape::LineOOShape(MPoint p1, MPoint p2) : Shape(p1, p2)
{
	const float circlePercent = 0.05;
	MPoint c1_pt1(
		static_cast<int>(p1.x * circlePercent + p2.x * (1 - circlePercent)),
		static_cast<int>(p1.y * circlePercent + p2.y * (1 - circlePercent))
	), c2_pt1(
		static_cast<int>(p1.x * (1 - circlePercent) + p2.x * circlePercent),
		static_cast<int>(p1.y * (1 - circlePercent) + p2.y * circlePercent)
	);
	MPoint c1_pt2 = MReflectPt(p1, c1_pt1),
		c2_pt2 = MReflectPt(p2, c2_pt1);

	c1 = EllipseShape(c1_pt1, c1_pt2);
	c2 = EllipseShape(c2_pt1, c2_pt2);
	ln = LineShape(p1, p2);
}

void LineOOShape::Render(HDC hdc)
{
	ln.Render(hdc);
	c1.Render(hdc);
	c2.Render(hdc);
}

LineOOShape::~LineOOShape()
{
	
}