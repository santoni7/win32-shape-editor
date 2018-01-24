#pragma once
#include <vector>
#include "XShape.h"
class XComposite : public XShape
{
protected:
	std::vector<XShape*> v;
public:
	XComposite(XShapeImpl* imp) : XShape(imp) { }
	virtual void Add(XShape* shape);
	virtual void Remove(XShape* shape);
	virtual XShape* GetChild(int n);
	virtual std::vector<XShape*> &GetChildren();
	XComposite* GetComposite() override { return this; }

	void Render(XContext&) override;
	void RenderSimple(XContext&) override;
	XShape* clone() override { return new XComposite(imp); }
};

