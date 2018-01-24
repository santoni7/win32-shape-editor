#pragma once
#include "XShapeImpl.h"
#include <vector>
class XPointImpl : public XShapeImpl {
public: //XPointImpl() = default;
		//void DoRender(XContext&, const MPoint& origin, const RenderData&);
};

class XPathShapeImpl : public XShapeImpl {
public:
	virtual void DoRender(XContext&, const MPoint& origin, const std::vector<MPoint>& points, const RenderData&, bool closed_path = false) = 0;
	
	/* Primitive drawing functions: */
	virtual void RenderLine(XContext&, MPoint, MPoint) = 0;
	virtual void RenderRect(XContext&, MPoint left_top, MPoint right_bottom) = 0;
	virtual void RenderEllipse(XContext&, MPoint left_top, MPoint right_bottom) = 0;
};