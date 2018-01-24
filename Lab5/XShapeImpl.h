#pragma once
#include "XContext.h"
class XShapeImpl
{
public:
	struct RenderData {
		XColor *outline_color;
		XColor *fill_color;
		RenderData() = default;
		RenderData(XColor *outline, XColor *fill) : outline_color(outline), fill_color(fill) { }
	};
	XShapeImpl();
	virtual ~XShapeImpl();
	
	virtual void BeforeRender(XContext&, const RenderData&) = 0;
	virtual void AfterRender(XContext&, const RenderData&) = 0;
	virtual void DoRender(XContext&,const MPoint& origin, const RenderData&) = 0;

	static RenderData CreateRenderData(XColor *outline_color, XColor *fill_color)
	{
		return RenderData{ outline_color, fill_color };
	}
};



