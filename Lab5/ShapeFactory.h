#pragma once
#include "all_shapes.h"
#include <type_traits>
#include <string>
class ShapeFactory
{

public:
	template <typename T> static
		typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type	*shape(MPoint p1, MPoint p2)
	{
		return new T(p1, p2);
	}
	template <typename T> static
		typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type	*shape()
	{
		return new T();
	}

	static Shape* fromWString(std::wstring sh, MPoint p1, MPoint p2) {
		int res = sh.find(_T("cube"));
		if (sh.find(_T("point")) == 0) 
			return shape<PointShape>(p1, p2);
		else if (sh.find(_T("lineoo")) == 0) //Notice: lineoo should be checked before line
			return shape<LineOOShape>(p1, p2);
		else if (sh.find(_T("line")) == 0)
			return shape<LineShape>(p1, p2);
		else if (sh.find(_T("rect")) == 0)
			return shape<RectShape>(p1, p2);
		else if (sh.find(_T("ellipse")) == 0)
			return shape<EllipseShape>(p1, p2);
		else if (sh.find(_T("cube")) == 0)
			return shape<CubeShape>(p1, p2);
		else return nullptr;
	}
};
