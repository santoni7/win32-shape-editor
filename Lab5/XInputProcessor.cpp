#include "stdafx.h"
#include "XInputProcessor.h"

bool XGdiTwoPointInputProcessor::OnMouseDown(XContext& ctx)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_context(ctx).hWnd, &pt);
	_start = MPoint(pt.x, pt.y);
	_pressed = true;
	return false;
}

bool XGdiTwoPointInputProcessor::OnMouseUp(XContext& ctx)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_context(ctx).hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
	_pressed = false;
	if (im == IM_CENTERCORNER)
	{
		_start = MReflectPt(_start, _end);
	}
	return true;
}

bool XGdiTwoPointInputProcessor::OnMouseMove(XContext& ctx)
{
	_prev = _end;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_context(ctx).hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
	return false;
}

std::vector<MPoint> XInputProcessor::Result()
{
	
}

XGdiContext& XGdiInputProcessor::_context(XContext& ctx)
{
	return dynamic_cast<XGdiContext&>(ctx);
}

void XGdiTwoPointInputProcessor::setInputMethod(InputMethod im)
{
	this->im = im;
}
