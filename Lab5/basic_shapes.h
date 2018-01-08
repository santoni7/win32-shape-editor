#pragma once
#include "stdafx.h"
#include "Helpers.h"
#include "shape.h"

#define SH_POINT_WIDTH 4
class PointShape : public Shape {
public:
	PointShape() { shouldFill = true; }
	PointShape(const PointShape& ps) : Shape(ps) { }
	PointShape(MPoint pt, MPoint ignored) : Shape(pt, MPoint()) { }
	PointShape(MPoint pt) : Shape(pt, MPoint()) { }
	const char* SimpleName() const override { return "point"; }
	Shape* copy() override;
	void RenderSimple(HDC hdc) override;
	~PointShape();
};
class LineShape : public Shape {
public:
	LineShape() {}
	LineShape(const PointShape& ls) : Shape(ls) { }
	LineShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	const char* SimpleName() const override { return "line"; }
	Shape* copy() override;
	void RenderSimple(HDC hdc) override;
	~LineShape();
};
class RectShape : public Shape {
public:
	RectShape() {}
	RectShape(const PointShape& rs) : Shape(rs) { }
	RectShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	const char* SimpleName() const override { return "rect"; }
	Shape* copy() override;
	void RenderSimple(HDC hdc) override;
	~RectShape();
};
class EllipseShape : public Shape {
public:
	EllipseShape() {}
	EllipseShape(const PointShape& es) : Shape(es) { }
	EllipseShape(MPoint p1, MPoint p2) : Shape(p1, p2) { }
	const char* SimpleName() const override { return "ellipse"; }
	Shape* copy() override;
	void RenderSimple(HDC hdc) override;
	~EllipseShape();
};

