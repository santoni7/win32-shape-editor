#pragma once
#include "Shapes.h"
#include <type_traits>

class ShapeFactory
{

public:
	static Shape* shape(const MPoint p1,const MPoint p2,const ShapeType st)
	{
		switch(st)
		{
		case ST_POINT:
			return shape<PointShape>(p1, p2);
		case ST_LINE:
			return shape<LineShape>(p1, p2);
		case ST_RECTANGLE:
			return shape<RectShape>(p1, p2);
		case ST_ELLIPSE:
			return shape<EllipseShape>(p1, p2);
		case ST_LINEOO:
			return shape<LineOOShape>(p1, p2);
		default:
			return nullptr;
		}
	}

	static Shape* shape(const MPoint p1, const MPoint p2, const EditorInfo einfo)
	{
		Shape *sh = nullptr;
		switch (einfo.st)
		{
		case ST_POINT:
			sh = shape<PointShape>(p1, p2);
			break;
		case ST_LINE:
			sh = shape<LineShape>(p1, p2);
			break;
		case ST_RECTANGLE:
			sh = shape<RectShape>(p1, p2);
			break;
		case ST_ELLIPSE:
			sh = shape<EllipseShape>(p1, p2);
			break;
		case ST_LINEOO:
			sh = shape<LineOOShape>(p1, p2);
			break;
		}
		if(sh)
		{
			sh->SetFillColor(einfo.fillCol, einfo.shouldFill);
			sh->SetOutlineColor(einfo.outlineCol);
		}
		return sh;
	}
	template <typename T> static
	typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type	*shape(MPoint p1, MPoint p2)
	{
		return new T(p1, p2);
	}
};
