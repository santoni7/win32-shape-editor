#pragma once
#include "XGdiShapeImpl.h"
#define SH_POINT_WIDTH 4
class XGdiPointShapeImpl : public XPointImpl, protected XGdiShapeImpl
{
	void DoRender(XContext& ctx, const MPoint& origin, const RenderData& rd) override {
		_render(_context(ctx), origin, _color(rd.fill_color), _color(rd.outline_color));
	}
	void _render(XGdiContext& ctx, const MPoint& origin, XGdiColor *fill_col, XGdiColor *out_col) override {
#ifdef SH_POINT_WIDTH
		const int w = SH_POINT_WIDTH;
		::Ellipse(ctx.hdc, origin.x - w / 2, origin.y - w / 2, origin.x + w / 2, origin.y + w / 2);
#else 
		::SetPixel(ctx.hdc, origin.x, origin.y, fill_col->cr);
#endif
	}
	void BeforeRender(XContext& ctx, const RenderData& rd) override { 
		XGdiShapeImpl::BeforeRender(ctx, rd); 
	}
	void AfterRender(XContext& ctx, const RenderData& rd) override { XGdiShapeImpl::AfterRender(ctx, rd); }

};
class XGdiPathShapeImpl : public XGdiShapeImpl, public XPathShapeImpl {
protected:
	virtual void _render(XGdiContext& ctx, const MPoint& origin, XGdiColor *fill_col, XGdiColor *outline_col) override {
		/* Not implemented */
	}
	virtual void _render(XGdiContext& ctx, const MPoint& origin, const std::vector<MPoint>& points, XGdiColor *fill_col, XGdiColor *outline_col, bool cycle);
public:
	XGdiPathShapeImpl() { };
	void DoRender(XContext&, const MPoint& origin, const RenderData&) override {
		/* Not implemented */
	}
	virtual void DoRender(XContext& ctx, const MPoint& origin, const std::vector<MPoint>& points, const RenderData& rd, bool closed_path = false) {
		_render(_context(ctx), origin, points, _color(rd.fill_color), _color(rd.outline_color), closed_path);
	}
	// TODO: Find why theese two lines make class non-abstract 
	void BeforeRender(XContext& ctx, const RenderData& rd) override { XGdiShapeImpl::BeforeRender(ctx, rd); }
	void AfterRender(XContext& ctx, const RenderData& rd) override { XGdiShapeImpl::AfterRender(ctx, rd); }
	void RenderLine(XContext&, MPoint, MPoint) override;
	void RenderRect(XContext&, MPoint left_top, MPoint right_bottom) override;
	void RenderEllipse(XContext&, MPoint left_top, MPoint right_bottom) override;
};

//class XGdiBasicShapesFactory {
//public:
//	virtual XShape * CreateShape() = 0;
//};
//class XGdiPathShapeFactory : XGdiBasicShapesFactory {
//public:
//	virtual  
//};