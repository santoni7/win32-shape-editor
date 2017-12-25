#pragma once
#include "shape.h"
#include "basic_shapes.h"
class LineOOShape : public Shape
{
	EllipseShape c1, c2;
	LineShape ln;
public:
	LineOOShape() {}
	LineOOShape(const LineOOShape& sh) : c1(sh.c1), c2(sh.c2), ln(sh.ln), Shape(sh) { }
	LineOOShape(MPoint p1, MPoint p2);
	Shape* copy() override;
	const char* SimpleName() const override { return "lineoo"; }
	void SetPoints(MPoint p1, MPoint p2) override;
	void Render(HDC hdc) override;
	void SetOutlineColor(COLORREF col) override;
	void SetFillColor(COLORREF col, bool shouldFill) override;
	void RenderSimple(HDC hdc) override;
	~LineOOShape();
};