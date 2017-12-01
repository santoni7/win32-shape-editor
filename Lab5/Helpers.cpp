#include "stdafx.h"
#include "Helpers.h"

MPoint operator+(const MPoint& lpt, const MPoint& rpt) {
	return MPoint(lpt.x + rpt.x, lpt.y + rpt.y);
}

MPoint operator-(const MPoint& pt) {
	return MPoint(-pt.x, -pt.y);
}

MPoint operator-(const MPoint& lpt, const MPoint& rpt) {
	return MPoint(lpt.x - rpt.x, lpt.y - rpt.y);
}

//
// ���������� ����� pt �� �������� ������ ������������ ������
//
int MReflectPt(int center, int point) {
	return 2 * center - point;
}

//
// ���������� ����� pt �� ��������� ������������ ������
//
MPoint MReflectPt(const MPoint& center, const MPoint& point) {
	return MPoint(MReflectPt(center.x, point.x), MReflectPt(center.y, point.y));
}
