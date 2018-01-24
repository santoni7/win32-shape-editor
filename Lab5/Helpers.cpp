#include "stdafx.h"
#include "Helpers.h"
#include <cmath>
#define SQR(x) ((x)*(x))

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

double MLineLength(const MPoint & p1, const MPoint & p2)
{
	return sqrt(SQR(p1.x-p2.x) + SQR(p1.y-p2.y));
}
