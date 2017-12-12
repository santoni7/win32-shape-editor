#include "stdafx.h"
#include "LineOOShape.h"

LineOOShape::LineOOShape(MPoint p1, MPoint p2) : Shape(p1, p2)
{
	SetPoints(p1, p2);
}
void LineOOShape::SetPoints(MPoint p1, MPoint p2) {
	int radius = (int)(MLineLength(p1, p2) * 0.15);
	MPoint radiusPt = MPoint(radius, radius);
	MPoint c1_pt1 = p1 - radiusPt,
		c1_pt2 = p1 + radiusPt,
		c2_pt1 = p2 - radiusPt,
		c2_pt2 = p2 + radiusPt;

	c1 = EllipseShape(c1_pt1, c1_pt2);
	c2 = EllipseShape(c2_pt1, c2_pt2);
	ln = LineShape(p1, p2);
}
Shape* LineOOShape::copy() {
	return new LineOOShape(*this);
}

void LineOOShape::Render(HDC hdc)
{
	ln.Render(hdc);
	c1.Render(hdc);
	c2.Render(hdc);
}

void LineOOShape::RenderSimple(HDC hdc)
{
	ln.RenderSimple(hdc);
	c1.RenderSimple(hdc);
	c2.RenderSimple(hdc);
}

void LineOOShape::SetOutlineColor(COLORREF color)
{
	c1.SetOutlineColor(color);
	c2.SetOutlineColor(color);
	ln.SetOutlineColor(color);
}

void LineOOShape::SetFillColor(COLORREF color, bool fill) {
	c1.SetFillColor(color, fill);
	c2.SetFillColor(color, fill);
}

LineOOShape::~LineOOShape()
{

}