#pragma once
#include "stdafx.h"
#include "Helpers.h"
#define SH_POINT_WIDTH 4

 class Shape {
protected:
	MPoint p1, p2;
	COLORREF outlineCol;
	COLORREF fillCol;
	bool shouldFill;
public:
	Shape(){}
	Shape(MPoint p1, MPoint p2)
		: p1(p1), p2(p2) { }
	void SetPoints(MPoint p1, MPoint p2);
	void SetPoints(int x1, int y1, int x2, int y2);
	void SetOutlineColor(COLORREF color);
	void SetFillColor(COLORREF color, bool shouldFill);

	virtual void Render(HDC hdc) = 0;
	virtual ~Shape() { };
	//static Shape* CreateShape(MPoint p1, MPoint p2, ShapeType shapeType, InputMethod inputMethod);
};

class PointShape : public Shape {
public:
	PointShape() {}
	PointShape(MPoint pt, MPoint ignored) : Shape(pt, MPoint()) { }
	PointShape(MPoint pt) : Shape(pt, MPoint()) { }
	void Render(HDC hdc) override;
	~PointShape();
};
class LineShape : public Shape {
public:
	LineShape() {}
	LineShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	void Render(HDC hdc) override;
	~LineShape();
};
class RectShape : public Shape { 
public:
	RectShape() {}
	RectShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	void Render(HDC hdc) override;
	~RectShape();
};
class EllipseShape : public Shape {
public:
	EllipseShape() {}
	EllipseShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	void Render(HDC hdc) override;
	~EllipseShape();
};

class LineOOShape : public Shape
{
	EllipseShape c1, c2;
	LineShape ln;
public:
	LineOOShape(){}
	LineOOShape(MPoint p1, MPoint p2);
	void Render(HDC hdc) override;
	~LineOOShape();
};