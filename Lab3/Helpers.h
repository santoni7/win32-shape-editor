#pragma once
#include "stdafx.h"

enum ShapeType { ST_POINT, ST_LINE, ST_ELLIPSE, ST_RECTANGLE };
enum InputMethod { IM_CENTERCORNER, IM_CORNERCORNER };

struct MPoint { 
	int x; 
	int y; 
	MPoint(int x=0, int y=0)
		: x(x), y(y) { }
};

struct MLine 
{
	MPoint p1; MPoint p2; 
	MLine(MPoint p1, MPoint p2)
		: p1(p1), p2(p2) { }
	MLine(int x1, int y1, int x2, int y2) {
		p1 = MPoint(x1, y1);
		p2 = MPoint(x2, y2);
	}
};

MPoint operator+(const MPoint& lpt, const MPoint& rpt);
MPoint operator-(const MPoint& pt);
MPoint operator-(const MPoint& lpt, const MPoint& rpt);

int MReflectPt(int center, int point);
MPoint MReflectPt(const MPoint& center, const MPoint& point);