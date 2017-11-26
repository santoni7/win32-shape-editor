#pragma once
#include "stdafx.h"
#include "Helpers.h"
#include "Shapes.h"

class Editor {
public:
	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	virtual void OnMouseMove() = 0;
	virtual ~Editor() { }
};

class ShapeEditor : public Editor
{
	MPoint _start, _end, _prev;
	bool _isPressed;
	HWND hWnd;
public:
	//ShapeEditor(): _isPressed(false), hWnd(nullptr)	{ }
	ShapeEditor(HWND hWnd): _isPressed(false), hWnd(hWnd) { }
	~ShapeEditor() = 0;

	MPoint start() const { return _start; }
	MPoint end() const { return _end; }
	MPoint prev() const { return _prev; }
	bool isPressed() const { return _isPressed; }

	void OnMouseDown() override;
	void OnMouseMove() override;
	void OnMouseUp() override;

	virtual Shape* shape() const = 0;
};

class LineEditor : public ShapeEditor
{
public:
	LineEditor(HWND hWnd) : ShapeEditor(hWnd) { }
	Shape* shape() const override
	{
		return new LineShape(start(), end());
	}
};

class PointEditor : public ShapeEditor
{
public:
	PointEditor(HWND hWnd) : ShapeEditor(hWnd) { }
	Shape* shape() const override
	{
		return new PointShape(end());
	}
};


class RectEditor : public ShapeEditor
{
public:
	RectEditor(HWND hWnd) : ShapeEditor(hWnd) { }
	Shape* shape() const override
	{
		return new RectShape(start(), end());
	}
	~RectEditor() {}
};

class EllipseEditor : public ShapeEditor
{
public:
	EllipseEditor(HWND hWnd) : ShapeEditor(hWnd) { }
	Shape* shape() const override
	{
		return new EllipseShape(start(), end());
	}
	~EllipseEditor(){}
};