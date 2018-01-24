#include "stdafx.h"
#include "XShape.h"
#include "XComposite.h"

XShape::XShape(XShapeImpl *imp)
{
	this->imp = imp;
	if (!imp) throw std::exception("Implementation can not be null!");
}


void XShape::GetColors(XColor *& outline, XColor *& fill)
{
	outline = outline_color;
	fill = fill_color;
}

XShape::~XShape()
{
}

void XShape::Render(XContext &_context) 
{
	XShapeImpl::RenderData rd{ outline_color, fill_color };
	imp->BeforeRender(_context, rd);
	imp->DoRender(_context, origin, rd);
	imp->AfterRender(_context, rd);
}
void XShape::RenderSimple(XContext &_context)
{
	XShapeImpl::RenderData rd{ outline_color, fill_color };
	imp->DoRender(_context, origin, rd);
}

XShape *XShape::clone()
{
	return new XShape(imp);
}

void XShape::SetColors(XColor *outlineColor, XColor *fillColor)
{
	if (outlineColor) this->outline_color = outlineColor;
	if (fillColor) this->fill_color = fillColor;
}
void XShape::Move(const MPoint& pt) {
	this->origin = pt;
}

void XShape::Offset(int dx, int dy) {
	this->origin.x += dx;
	this->origin.y += dy;
}


