#pragma once
#include "stdafx.h"
#include "Helpers.h"
//#include "shape.h"
#include <type_traits>

class InputProcessor {
public:
	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	virtual void OnMouseMove() = 0;
	virtual ~InputProcessor() { }
};

class TwoPointInputProcessor : public InputProcessor
{

protected:
	MPoint _start, _end, _prev;
	bool _pressed;
	HWND hWnd;
	InputMethod im = IM_CORNERCORNER;
public:
	//TwoPointInputProcessor(): _pressed(false), hWnd(nullptr)	{ }
	TwoPointInputProcessor(HWND hWnd) : _pressed(false), hWnd(hWnd) { }
	~TwoPointInputProcessor();

	void setInputMethod(InputMethod im);

	MPoint start() const { return _start; }
	MPoint end() const { return _end; }
	MPoint prev() const { return _prev; }
	bool isPressed() const { return _pressed; }

	void OnMouseDown() override;
	void OnMouseMove() override;
	void OnMouseUp() override;
};