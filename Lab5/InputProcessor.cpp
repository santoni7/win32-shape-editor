#include "stdafx.h"
#include "InputProcessor.h"

void TwoPointInputProcessor::OnMouseDown()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_start = MPoint(pt.x, pt.y);
	_pressed = true;
}

void TwoPointInputProcessor::OnMouseUp()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
	_pressed = false;
	if(im == IM_CENTERCORNER)
	{
		_start = MReflectPt(_start, _end);
	}
}

void TwoPointInputProcessor::OnMouseMove()
{
	_prev = _end;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
}

void TwoPointInputProcessor::setInputMethod(InputMethod im)
{
	this->im = im;
}

TwoPointInputProcessor::~TwoPointInputProcessor()
{
	this->hWnd = nullptr;
}