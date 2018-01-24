#include "stdafx.h"
#include "XContext.h"
MPoint operator+(const MPoint& lpt, const MPoint& rpt) {
	return MPoint(lpt.x + rpt.x, lpt.y + rpt.y);
}

MPoint operator-(const MPoint& pt) {
	return MPoint(-pt.x, -pt.y);
}

MPoint operator-(const MPoint& lpt, const MPoint& rpt) {
	return MPoint(lpt.x - rpt.x, lpt.y - rpt.y);
}