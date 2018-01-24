#pragma once
#include "XShapeImpl.h"
#include "XGdiContext.h"
#include <vector>
class XGdiShapeImpl : public XShapeImpl {
protected:
	HBRUSH hBrush, hBrushOld;
	HPEN hPenOld, hPen;
	virtual void _beforeRender(XGdiContext& ctx, XGdiColor *fill_col, XGdiColor *outline_col);
	virtual void _afterRender(XGdiContext& ctd);
	virtual void _render(XGdiContext& ctx, const MPoint& origin, XGdiColor *fill_col, XGdiColor *outline_col) = 0;
	XGdiContext& _context(XContext& ctx);
	XGdiColor* _color(XColor* col);
public:
	virtual void BeforeRender(XContext&, const RenderData&) override;
	virtual void DoRender(XContext&, const MPoint& origin, const RenderData&) override;
	virtual void AfterRender(XContext&, const RenderData&) override;
};
//
//class XGdiMultipointShapeImpl : public XGdiShapeImpl, public XMultipointShapeImpl {
//protected:
//	virtual void _render(XGdiContext& ctx, const MPoint& origin, const std::vector<MPoint>& points, XGdiColor *fill_col, XGdiColor *outline_col) = 0;
//public:
//	virtual void DoRender(XContext&, const MPoint& origin, const std::vector<MPoint>& points, const RenderData&);
//};
//
