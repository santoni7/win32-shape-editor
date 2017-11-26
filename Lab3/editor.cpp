#include "stdafx.h"
#include "Editor.h"

void ShapeEditor::OnMouseDown()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_start = MPoint(pt.x, pt.y);
	_isPressed = true;
}

void ShapeEditor::OnMouseUp()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
	_isPressed = false;
}

void ShapeEditor::OnMouseMove()
{
	_prev = _end;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->hWnd, &pt);
	_end = MPoint(pt.x, pt.y);
}

ShapeEditor::~ShapeEditor()
{
	
}