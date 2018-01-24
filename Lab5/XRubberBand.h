#pragma once
#include "XComposite.h"
#include "ShapeFactory.h"

class XRubberBand : public XComposite
{
	XBasicShapeFactory factory;
public:
	XRubberBand(XShapeImpl* imp) : XComposite(imp)
	{
		
	}
	void AddLine(MPoint p1, MPoint p2)
	{
		Add(factory.CreateLine(p1, p2));
	}
};
