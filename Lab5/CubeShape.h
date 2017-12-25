#pragma once
#include "shape.h"
#include "Helpers.h"
class CubeShape :
	public Shape
{
	int width = 0;
	void renderLine(HDC hdc, MPoint p1, MPoint p2);
public:
	CubeShape();
	CubeShape(const CubeShape& cs) : width(cs.width), Shape(cs) { }
	CubeShape(MPoint p1, MPoint p2);
	void SetPoints(MPoint p1, MPoint p2) override;
	Shape* copy() override;
	const char* SimpleName() const override { return "cube"; }
	void RenderSimple(HDC hdc) override;
	~CubeShape();
};

