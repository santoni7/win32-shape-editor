#pragma once
#include "XGlobal.h"
#include "XGdiGlobal.h"
int MReflectPt(int center, int point);
MPoint MReflectPt(const MPoint& center, const MPoint& point);
double MLineLength(const MPoint& p1, const MPoint& p2);
class XInputProcessor
{
protected:
	MouseState mouseState, lastMouseState, onLastLBDown;
	virtual void PushPoint(MPoint pt);
	virtual MPoint PopPoint();
public:
	XInputProcessor() = default;
	virtual ~XInputProcessor() = default;

	virtual bool Ready() = 0;
	virtual std::vector<MPoint> Result();
	virtual bool OnMouseDown(XContext&)
	{
		mouseState.LButton = Pressed;
		return false;
	}
	virtual bool OnMouseMove(XContext&) { return false; }
	virtual bool OnMouseUp(XContext&)
	{
		mouseState.LButton = Released;
		return false;
	}
	virtual bool OnRMouseDown(XContext&) { return false; }
	virtual bool OnRMouseUp(XContext&) { return false; }
	virtual bool OnDoubleClick(XContext&) { return false; }
};
class XGdiInputProcessor : public XInputProcessor
{
protected:
	XGdiContext& _context(XContext&);
};
#define PTCOUNT 15
class XPathInputProcessor: public XGdiInputProcessor
{
protected:
	bool _pressed;
	std::vector<MPoint> points, prev;

public:
	MPoint cursor_pos(XGdiContext& ctx)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(ctx.hWnd, &pt);
		return {pt.x, pt.y};
	}
	XPathInputProcessor(XContext& ctx) : XGdiInputProcessor()
	{
		_pressed = false;
	} 
	std::vector<MPoint> get_points(int n_last = -1)
	{
		if(n_last < 0 || n_last >= points.size())
			return points;
		return std::vector<MPoint>(points.begin() + points.size() - n_last, points.end());
	}
	std::vector<MPoint> get_prev_points(int n_last = -1)
	{
		if (n_last < 0 || n_last >= prev.size())
			return prev;
		return std::vector<MPoint>(prev.begin() + prev.size() - n_last, prev.end());
	}
	bool isPressed() { return _pressed;  }
	bool OnMouseDown(XContext& ctx) override
	{
		if (points.size() >= PTCOUNT ) points.clear();
		
		points.push_back(cursor_pos(_context(ctx)));		
		_pressed = true; return false;
	}
	bool OnMouseMove(XContext& ctx) override
	{
		prev = points;
		if(!points.empty())
			*points.rbegin() = cursor_pos(_context(ctx));
		return false;
	}
	bool OnMouseUp(XContext& ctx) override
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(_context(ctx).hWnd, &pt);
		*points.rbegin() = MPoint( pt.x, pt.y );
		return points.size() >= PTCOUNT;
		
	}
};


class XGdiTwoPointInputProcessor: public XGdiInputProcessor {
protected:
	MPoint _start, _end, _prev;
	bool _pressed;
	InputMethod im = IM_CORNERCORNER;
public:
	//TwoPointInputProcessor(): _pressed(false), hWnd(nullptr)	{ }
	XGdiTwoPointInputProcessor() : _pressed(false) { }
	~XGdiTwoPointInputProcessor() = default;

	void setInputMethod(InputMethod im);

	MPoint start() const { return _start; }
	MPoint end() const { return _end; }
	MPoint prev() const { return _prev; }
	MPoint prev_start() const {
		return im == IM_CENTERCORNER ? MReflectPt(start(), prev()) : start();
	}
	bool isPressed() const { return _pressed; }

	bool OnMouseDown(XContext&) override;
	bool OnMouseMove(XContext&) override;
	bool OnMouseUp(XContext&) override;
};