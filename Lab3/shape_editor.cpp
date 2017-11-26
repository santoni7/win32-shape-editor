#include "stdafx.h"
#include "shape_editor.h"
#include "Shapes.h"
#define SHAPES_ARRAY_SIZE 123

Shape *SHAPES[SHAPES_ARRAY_SIZE];
int cur = 0;

ShapeEditorController::ShapeEditorController(HWND hWnd)
{
	this->hWnd = hWnd;
	this->editor = new PointEditor(hWnd);
}

ShapeEditorController::~ShapeEditorController()
{
	this->hWnd = nullptr;
}

void ShapeEditorController::OnMouseDown() const
{
	editor->OnMouseDown();
}

void ShapeEditorController::OnMouseUp() const
{
	editor->OnMouseUp();
	Shape* shape = editor->shape();
	shape->SetFillColor(fillCol, shouldFill);
	shape->SetOutlineColor(outlineCol);
	SHAPES[cur++] = shape;
	if (cur == SHAPES_ARRAY_SIZE) 
		cur--;
	::InvalidateRect(hWnd, nullptr, true);
}

void ShapeEditorController::OnMouseMove() const
{
	editor->OnMouseMove();
	if (editor->isPressed()) {
		DrawRubberBand();
	}
}

void ShapeEditorController::OnPaint() const
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	for (int i = 0; i < cur; ++i) {
		SHAPES[i]->Render(hdc);
	}
	EndPaint(hWnd, &ps);
}

void ShapeEditorController::DrawRubberBand() const
{
	MPoint start = editor->start(),
		end = editor->end(),
		prev = editor->prev();
	//Get a client DC.
	HDC hdc = ::GetDC(hWnd);
	::SetROP2(hdc, R2_NOTXORPEN);
	::SelectObject(hdc, ::CreatePen(PS_DOT, 1, 0));
	if (shapeType == ST_RECTANGLE) {
		::Rectangle(hdc, start.x, start.y, prev.x, prev.y);
		::Rectangle(hdc, start.x, start.y, end.x, end.y);
	}
	else if (shapeType == ST_ELLIPSE) {
		::Ellipse(hdc, start.x, start.y, prev.x, prev.y);
		::Ellipse(hdc, start.x, start.y, end.x, end.y);
	}
	else if(shapeType == ST_LINE)
	{
		::MoveToEx(hdc, start.x, start.y, nullptr);
		::LineTo(hdc, prev.x, prev.y);
		::MoveToEx(hdc, start.x, start.y, nullptr);
		::LineTo(hdc, end.x, end.y);
	}
	::ReleaseDC(hWnd, hdc);
}


// Setters: 

void ShapeEditorController::SetShapeType(ShapeType shapeType)
{
	this->shapeType = shapeType;
	switch (shapeType)
	{
	case ST_POINT:
		SetEditor(new PointEditor(hWnd));
		break;
	case ST_LINE:
		SetEditor(new LineEditor(hWnd));
		break;
	case ST_RECTANGLE:
		SetEditor(new RectEditor(hWnd));
		break;
	case ST_ELLIPSE:
		SetEditor(new EllipseEditor(hWnd));
		break;
	}
}

void ShapeEditorController::SetEditor(ShapeEditor* editor)
{
	this->editor = editor;
}

void ShapeEditorController::SetInputMethod(InputMethod inputMethod)
{
	this->inputMethod = inputMethod;
}

void ShapeEditorController::SetOutlineColor(COLORREF color)
{
	this->outlineCol = color;
}

void ShapeEditorController::SetFillColor(COLORREF color, bool shouldFill)
{
	this->fillCol = color;
	this->shouldFill = shouldFill;
}