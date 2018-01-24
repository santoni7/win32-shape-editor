#include "stdafx.h"
#include "XBasicShapes.h"
XLine::XLine(XPathShapeImpl* imp, MPoint p1, MPoint p2) : XPathShape(imp) {
	points = { p1, p2 };
};
void XLine::Render(XContext &ctx) {
	XShapeImpl::RenderData rd(outline_color, fill_color);
	_imp()->BeforeRender(ctx, rd);
	_imp()->DoRender(ctx, origin, points, rd, false);
	_imp()->AfterRender(ctx, rd);
}

void XLine::RenderSimple(XContext & ctx)
{
	_imp()->RenderLine(ctx, points[0], points[1]);
}

XRectangle::XRectangle(XPathShapeImpl* imp, MPoint lt, MPoint rb) : XPathShape(imp) {
	points = { lt, rb };
}

void XRectangle::Render(XContext &ctx) {
	XShapeImpl::RenderData rd(outline_color, fill_color);
	_imp()->BeforeRender(ctx, rd);
	_imp()->RenderRect(ctx, points[0], points[1]);	
	_imp()->AfterRender(ctx, rd);
}

void XRectangle::RenderSimple(XContext &ctx) {
	_imp()->RenderRect(ctx, points[0], points[1]);
}
XEllipse::XEllipse(XPathShapeImpl *imp, MPoint lt, MPoint rb) : XPathShape(imp) {
	points = { lt, rb };
}

void XEllipse::Render(XContext &ctx) {
	const XShapeImpl::RenderData rd(outline_color, fill_color);
	_imp()->BeforeRender(ctx, rd);
	_imp()->RenderEllipse(ctx, points[0], points[1]);
	_imp()->AfterRender(ctx, rd);
}
void XEllipse::RenderSimple(XContext &ctx) {
	_imp()->RenderEllipse(ctx, points[0], points[1]);
}

XLineOO::XLineOO(XPathShapeImpl* imp, MPoint p1, MPoint p2) : XComposite(imp), XPathShape(imp)
{
	points = { p1, p2 };
	XBasicShapeFactory factory;
	int radius = (int)(MLineLength(p1, p2) * 0.15);
	MPoint radiusPt = MPoint(radius, radius);
	MPoint c1_pt1 = p1 - radiusPt,
		c1_pt2 = p1 + radiusPt,
		c2_pt1 = p2 - radiusPt,
		c2_pt2 = p2 + radiusPt;
	this->Add(factory.CreateLine(p1, p2));
	this->Add(factory.CreateEllipse(c1_pt1, c1_pt2));
	this->Add(factory.CreateEllipse(c2_pt1, c2_pt2));
}
void XLineOO::Render(XContext& ctx)
{
	XComposite::Render(ctx);
}
void XLineOO::RenderSimple(XContext& ctx)
{
	XComposite::RenderSimple(ctx);
}

void XLineOO::SetPoints(std::vector<MPoint> points)
{
	XPathShape::SetPoints(points);
	if (points.size() < 2) return;
	MPoint p1 = points[0], p2 = points[1];
	XBasicShapeFactory factory;
	int radius = (int)(MLineLength(p1, p2) * 0.15);
	MPoint radiusPt = MPoint(radius, radius);
	MPoint c1_pt1 = p1 - radiusPt,
		c1_pt2 = p1 + radiusPt,
		c2_pt1 = p2 - radiusPt,
		c2_pt2 = p2 + radiusPt;
	auto children = XComposite::GetChildren();
	if (children.size() < 3) return;
	XPathShape* ln = dynamic_cast<XPathShape*>(children[0]),
		*c1 = dynamic_cast<XPathShape*>(children[1]),
		*c2 = dynamic_cast<XPathShape*>(children[2]);
	if(ln) ln->SetPoints(points);
	if(c1) c1->SetPoints({ c1_pt1, c1_pt2 });
	if(c2) c2->SetPoints({ c2_pt1, c2_pt2 });
}

void XPathShape::Render(XContext &ctx)
{
	const XShapeImpl::RenderData rd(outline_color, fill_color);
	_imp()->BeforeRender(ctx, rd);
	_imp()->DoRender(ctx, origin, points, rd, closed_path);;
	_imp()->AfterRender(ctx, rd);
}

void XPathShape::RenderSimple(XContext &ctx)
{
	const XShapeImpl::RenderData rd(nullptr, nullptr);
	_imp()->DoRender(ctx, origin, points, rd, closed_path);
}
//void XPathShape::RenderSimple(XContext& ctx, int start_point, int end_point)
//{
//	const XShapeImpl::RenderData rd(nullptr, nullptr);
//	start_point = min(start_point, points.size());
//	end_point = min(end_point, points.size());
//	if (start_point < points.size() && end_point)
//	{
//		std::vector<MPoint> pts(points.begin() + start_point, points.begin() + end_point);
//		_imp()->DoRender(ctx, origin, pts, rd, closed_path);
//	}
//}
