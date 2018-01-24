#pragma once
#include "XContext.h"
#include "XShapeImpl.h"
class XComposite;

/* Basic abstract shape interface */
class XShape
{
protected:
	XShapeImpl *imp;
	MPoint origin;
	XColor *outline_color = nullptr;
	XColor *fill_color = nullptr;
public:
	XShape(XShapeImpl*);
	virtual XComposite* GetComposite() { return nullptr; } // By default, shape is not composite

	virtual void Move(const MPoint&);
	virtual void Offset(int dx, int dy);
	virtual void Render(XContext&);
	virtual void RenderSimple(XContext&);
	virtual XShape* clone();
	virtual void SetColors(XColor *outlineColor = nullptr, XColor *fillColor = nullptr);
	virtual void GetColors(XColor*& outline, XColor*& fill);
	virtual ~XShape();
};

