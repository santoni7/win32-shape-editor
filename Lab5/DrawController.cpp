#include "stdafx.h"
#include "DrawController.h"
#include "all_shapes.h"
#include <algorithm>
#include "strings.h"
#include <typeinfo>
#include "shape.h"
#include <iostream>
#define SHAPES_ARRAY_SIZE 123

//Shape *SHAPES[SHAPES_ARRAY_SIZE];
//int cur = 0;

DrawController::DrawController(HWND hWnd)
{
	this->hWnd = hWnd;
	this->inputProcessor = new TwoPointInputProcessor(hWnd);
}

DrawController::~DrawController()
{
	this->hWnd = nullptr;
}

int DrawController::GetShapesCount() const 
{
	return cur;
}

CustomTableData * DrawController::exportData() const
{
	auto pData = new CustomTableData;
	pData->cntCol = 5;
	pData->cntRow = cur;
	pData->colW = new int[5]{ 90,50,50,50,50 };
	//pData->rowH = new int[cur];
	pData->width = 90 + 50 * 4;
	pData->height = 80;
	pData->rows;// = std::vector<CustomTableRow*>(cur + 1);
	for (int i = 0; i<=cur; ++i)
	{
		CustomTableRow *row = new CustomTableRow;
		strings* str = strings::instance();
		row->height = 25;
		if (i == 1) row->selected = true;
		else row->selected = false;
		row->cells.push_back(new CustomTableCell(str->get(shapes.at(i)->SimpleName())));
		row->cells.push_back(new CustomTableCell(str->format("numfmt", shapes.at(i)->p1.x)));
		row->cells.push_back(new CustomTableCell(str->format("numfmt", shapes.at(i)->p1.y)));
		row->cells.push_back(new CustomTableCell(str->format("numfmt", shapes.at(i)->p2.x)));
		row->cells.push_back(new CustomTableCell(str->format("numfmt", shapes.at(i)->p2.y)));
		pData->rows.push_back(row);
	}
	return pData;
}

void DrawController::Start(Shape* sh) 
{
	if (cur >= shapes.size()) {
		reallocate();
	}
	shapes[cur] = sh;
}


void DrawController::Start(Shape* sh, InputMethod im, COLORREF outline, COLORREF fill, bool shouldFill) 
{
	Start(sh);
	SetInputMethod(im);
	SetOutlineColor(outline);
	SetFillColor(fill, shouldFill);
}

Shape* DrawController::current() const 
{
	return shapes[cur];
}

bool DrawController::reallocate() {
	//if (cur < shapes_size) return false;
	shapes.resize(shapes.size() + shapes_size_step);
	return true;
}

void DrawController::OnMouseDown() const
{
	inputProcessor->OnMouseDown();
}

void DrawController::OnMouseUp()
{
	inputProcessor->OnMouseUp();
	if (shapes[cur]) shapes[cur]->SetPoints(inputProcessor->start(), inputProcessor->end());
	if (shapes[cur]) shapes[cur]->SetFillColor(einfo.fillCol, einfo.shouldFill);
	if (shapes[cur]) shapes[cur]->SetOutlineColor(einfo.outlineCol);
	
	cur++;
	if (cur >= shapes.size()) 
		reallocate();
	//shapes[cur] = allocator.allocate(1);
	//Shape* new_shape = &Shape(*shapes[cur - 1]);
	shapes[cur] = shapes[cur-1]->copy();

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
		if(shapes[i]) shapes[i]->Render(hdc);
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
	
	Shape* curr = shapes[cur];
	if (curr)
	{
		curr->SetPoints(start, end);
		curr->RenderSimple(hdc);
		curr->SetPoints(prev_start, prev);
		curr->RenderSimple(hdc);
	}

	::ReleaseDC(hWnd, hdc);
}


// Setters: 
void DrawController::SetInputMethod(InputMethod inputMethod)
{
	einfo.im = inputMethod;
	inputProcessor->setInputMethod(einfo.im);
}

//void DrawController::SetShapeType(ShapeType shapeType)
//{
//	this->einfo.st = shapeType;
//}

void DrawController::SetOutlineColor(COLORREF color)
{
	this->einfo.outlineCol = color;
}

void DrawController::SetFillColor(COLORREF color, bool shouldFill)
{
	this->einfo.fillCol = color;
	this->einfo.shouldFill = shouldFill;
}