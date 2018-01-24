#pragma once
#include "XShape.h"
#include "XBasicShapesImpl.h"
#include "XImplStorage.h"
#include <vector>
#include "XComposite.h"

class XPoint : public XShape {
	public:
	XPoint(XShapeImpl* imp, MPoint origin) : XShape(imp) {
		Move(origin);
	}
	XShape* clone() override {
		return new XPoint(imp, origin);
	}
};

/* Shape that can be described by a set of points */
class XPathShape : public XShape {
protected:
	std::vector<MPoint> points;
	bool closed_path;
	XPathShapeImpl* _imp() { 
		if (!imp) return nullptr;
		return dynamic_cast<XPathShapeImpl*>(imp); 
	}
public:
	XShape* clone() override { return new XPathShape(_imp()); }
	void Render(XContext&) override;
	void RenderSimple(XContext&) override;
	virtual std::vector<MPoint>& Points() { return points; }
	virtual void SetPoints(std::vector<MPoint> points) { this->points = points; }
	XPathShape(XPathShapeImpl* imp) : XShape(imp) { }
	XPathShape(XPathShapeImpl* imp, std::vector<MPoint> &points, bool closed_path) : XShape(imp), closed_path(closed_path), points(points) { }
};

/* Line shape */
class XLine : public XPathShape {
public:
	XLine(XPathShapeImpl* imp, MPoint p1, MPoint p2);
	void Render(XContext &ctx) override;
	void RenderSimple(XContext &ctx) override;
	XShape* clone() override { 
		return new XLine(_imp(), this->points[0], this->points[1]); 
	}

}; 

/* Rectangle shape */
class XRectangle : public XPathShape {
public:
	XRectangle(XPathShapeImpl* imp, MPoint lt, MPoint rb);
	void Render(XContext &ctx) override;
	void RenderSimple(XContext &ctx) override;
	XShape* clone() override { return new XRectangle(_imp(), this->points[0], this->points[1]); }
};

/* Ellipse shape */
class XEllipse : public XPathShape {
public:
	XEllipse(XPathShapeImpl *imp, MPoint lt, MPoint rb);
	void Render(XContext &ctx) override;
	void RenderSimple(XContext &ctx) override;
	XShape* clone() override { return new XEllipse(_imp(), this->points[0], this->points[1]); }
};

/* O-O Shape */
class XLineOO : public XComposite, public XPathShape
{
public:
	void Render(XContext &ctx) override;
	void RenderSimple(XContext &ctx) override;
	void SetPoints(std::vector<MPoint> points) override;
	XLineOO(XPathShapeImpl *imp, MPoint p1, MPoint p2);
	XShape* clone() override { return (XComposite*)new XLineOO(_imp(), points[0], points[1]); }
};

/* XShape factory: */
class XBasicShapeFactory {
public:
	virtual ~XBasicShapeFactory() = default;
	XBasicShapeFactory() = default;
	XImplStorage *storage()	{
		return XImplStorage::inst();
	}
	virtual XShape* CreatePoint(MPoint pt = { 0, 0 }) {
		return new XPoint(storage()->GetImpl<XPointImpl>(), pt);
	}

	virtual XPathShape* CreateLine(MPoint p1 = { 0, 0 }, MPoint p2 = { 0, 0 }) {
		return new XLine(storage()->GetImpl<XPathShapeImpl>(), p1, p2);
	}

	virtual XPathShape* CreateRectangle(MPoint left_top = { 0, 0 }, MPoint right_bottom = { 0, 0 }) {
		return new XRectangle(storage()->GetImpl<XPathShapeImpl>(), left_top, right_bottom);
	}

	virtual XPathShape* CreateEllipse(MPoint left_top = { 0, 0 }, MPoint right_bottom = { 0, 0 }) {
		return new XEllipse(storage()->GetImpl<XPathShapeImpl>(), left_top, right_bottom);
	}

	virtual XPathShape* CreatePath(std::vector<MPoint> v, bool closed_path)
	{
		return new XPathShape(storage()->GetImpl<XPathShapeImpl>(), v, closed_path);
	}

	virtual XPathShape* CreateLineOO(MPoint left_top = { 0, 0 }, MPoint right_bottom = { 0, 0 })
	{
		return new XLineOO(storage()->GetImpl<XPathShapeImpl>(), left_top, right_bottom);
	}
};