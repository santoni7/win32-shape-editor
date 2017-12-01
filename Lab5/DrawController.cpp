#include "stdafx.h"
#include "DrawController.h"
#include "Shapes.h"
#include "ShapeFactory.h"
#define SHAPES_ARRAY_SIZE 123

Shape *SHAPES[SHAPES_ARRAY_SIZE];
int cur = 0;

DrawController::DrawController(HWND hWnd)
{
	this->hWnd = hWnd;
	this->inputProcessor = new TwoPointInputProcessor(hWnd);
}

DrawController::~DrawController()
{
	this->hWnd = nullptr;
}

void DrawController::OnMouseDown() const
{
	inputProcessor->OnMouseDown();
}

void DrawController::OnMouseUp() const
{
	inputProcessor->OnMouseUp();
	Shape *shape = ShapeFactory::shape(inputProcessor->start(), inputProcessor->end(), einfo);

	SHAPES[cur++] = shape;
	if (cur == SHAPES_ARRAY_SIZE) 
		cur--;
	::InvalidateRect(hWnd, nullptr, true);
}

void DrawController::OnMouseMove() const
{
	inputProcessor->OnMouseMove();
	if (inputProcessor->isPressed()) {
		DrawRubberBand();
	}
}

void DrawController::OnPaint() const
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);
	for (int i = 0; i < cur; ++i) {
		SHAPES[i]->Render(hdc);
	}
	::EndPaint(hWnd, &ps);
}

void DrawController::Undo()
{
	if(cur>0)
	{
		cur--;
		::InvalidateRect(hWnd, nullptr, true);
	}
}


void DrawController::DrawRubberBand() const
{
	MPoint start = inputProcessor->start(),
		end = inputProcessor->end(),
		prev = inputProcessor->prev(),
		prev_start = inputProcessor->start();
	if (einfo.im == IM_CENTERCORNER) {
		start = MReflectPt(start, end);
		prev_start = MReflectPt(inputProcessor->start(), prev);
	}
	HDC hdc = ::GetDC(hWnd);
	::SetROP2(hdc, R2_NOTXORPEN);
	::SelectObject(hdc, ::CreatePen(PS_DOT, 1, 0));
	switch (einfo.st){
	case ST_POINT:
		//Do nothing
		break;
	case ST_RECTANGLE:
		::Rectangle(hdc, prev_start.x, prev_start.y, prev.x, prev.y);
		::Rectangle(hdc, start.x, start.y, end.x, end.y);
		break;
	case ST_ELLIPSE:
		::Ellipse(hdc, prev_start.x, prev_start.y, prev.x, prev.y);
		::Ellipse(hdc, start.x, start.y, end.x, end.y);
		break;
	case ST_LINE:
		::MoveToEx(hdc, prev_start.x, prev_start.y, nullptr);
		::LineTo(hdc, prev.x, prev.y);
		::MoveToEx(hdc, start.x, start.y, nullptr);
		::LineTo(hdc, end.x, end.y);
		break;
	case ST_LINEOO:
		/*
		 * TODO: Draw rubber band for LineOO
		 */
		break;


	}
	::ReleaseDC(hWnd, hdc);
}


// Setters: 
void DrawController::SetInputMethod(InputMethod inputMethod)
{
	einfo.im = inputMethod;
	inputProcessor->setInputMethod(einfo.im);
}

void DrawController::SetShapeType(ShapeType shapeType)
{
	this->einfo.st = shapeType;
}

void DrawController::SetOutlineColor(COLORREF color)
{
	this->einfo.outlineCol = color;
}

void DrawController::SetFillColor(COLORREF color, bool shouldFill)
{
	this->einfo.fillCol = color;
	this->einfo.shouldFill = shouldFill;
}