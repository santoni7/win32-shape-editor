#include "stdafx.h"
#include "XGdiBasicShapesImpl.h"

void XGdiPathShapeImpl::RenderLine(XContext &ctx, MPoint p1, MPoint p2)
{
	XGdiContext &gctx = _context(ctx);
	if (!gctx.hdc) return;
	::MoveToEx(gctx.hdc, p1.x, p1.y, nullptr);
	::LineTo(gctx.hdc, p2.x, p2.y);
}

void XGdiPathShapeImpl::RenderRect(XContext &ctx, MPoint p1, MPoint p2) 
{
	::Rectangle(_context(ctx).hdc, p1.x, p1.y, p2.x, p2.y);
}

void XGdiPathShapeImpl::RenderEllipse(XContext &ctx, MPoint p1, MPoint p2)
{
	::Ellipse(_context(ctx).hdc, p1.x, p1.y, p2.x, p2.y);
}
void XGdiPathShapeImpl:: _render(XGdiContext& ctx, const MPoint& origin, const std::vector<MPoint>& points, XGdiColor *fill_col, XGdiColor *outline_col, bool closed_path) {
	int n = points.size();

	for (int i = 0; i < points.size(); ++i) {
		MoveToEx(ctx.hdc, points[i].x, points[i].y, nullptr);
		if (closed_path || i < points.size() - 1)
			LineTo(ctx.hdc, points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y);
	}
}