#pragma once
#include "stdafx.h"

enum ShapeType { ST_POINT, ST_LINE, ST_ELLIPSE, ST_RECTANGLE, ST_LINEOO, ST_CUBE };
enum InputMethod { IM_CENTERCORNER, IM_CORNERCORNER };

struct MPoint { 
	int x; 
	int y; 
	MPoint(int x=0, int y=0)
		: x(x), y(y) { }
};



MPoint operator+(const MPoint& lpt, const MPoint& rpt);
MPoint operator-(const MPoint& pt);
MPoint operator-(const MPoint& lpt, const MPoint& rpt);

int MReflectPt(int center, int point);
MPoint MReflectPt(const MPoint& center, const MPoint& point);
double MLineLength(const MPoint& p1, const MPoint& p2);