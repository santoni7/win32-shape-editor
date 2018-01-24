#include "stdafx.h"
#include "XGdiShapeImpl.h"
void XGdiShapeImpl::BeforeRender(XContext &ctx, const RenderData& rd) {
	XGdiColor *fcol = dynamic_cast<XGdiColor*>(rd.fill_color);
	XGdiColor *outcol = dynamic_cast<XGdiColor*>(rd.outline_color);

	_beforeRender(_context(ctx), fcol, outcol);
}
void XGdiShapeImpl::AfterRender(XContext& ctx, const RenderData&) {
	_afterRender(_context(ctx));
}
void XGdiShapeImpl::DoRender(XContext& ctx, const MPoint& origin, const RenderData& rd)
{
	_render(_context(ctx), origin, _color(rd.fill_color), _color(rd.outline_color));
}
void XGdiShapeImpl::_beforeRender(XGdiContext& ctx, XGdiColor *fill_col, XGdiColor *outline_col) {

	if (!(fill_col || outline_col)) return;

	hBrush = (HBRUSH)(fill_col->transparent
		? ::GetStockObject(NULL_BRUSH)
		: ::CreateSolidBrush(fill_col->cr));
	hBrushOld = (HBRUSH)::SelectObject(ctx.hdc, hBrush);

	hPen = ::CreatePen(PS_SOLID, 1, outline_col->cr);
	hPenOld = (HPEN)::SelectObject(ctx.hdc, hPen);
}

void XGdiShapeImpl::_afterRender(XGdiContext& ctx)
{
	::SelectObject(ctx.hdc, hBrushOld);
	::DeleteObject(hBrush);
	::SelectObject(ctx.hdc, hPenOld);
	::DeleteObject(hPen);
}

XGdiContext & XGdiShapeImpl::_context(XContext & ctx)
{
	return dynamic_cast<XGdiContext&>(ctx);
}

XGdiColor * XGdiShapeImpl::_color(XColor * col)
{
	return dynamic_cast<XGdiColor*>(col);
}

//void XGdiMultipointShapeImpl::DoRender(XContext& ctx, const MPoint &origin, const std::vector<MPoint>& points, const RenderData & rd)
//{
//	_render(context(ctx), origin, points, color(rd.fill_color), color(rd.outline_color));
//}
