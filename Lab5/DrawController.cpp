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

std::vector<Shape*>& DrawController::GetShapes()
{
	return this->shapes;
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

void DrawController::PaintDoubleBuffer(PAINTSTRUCT* pPaintStruct) const

{
	int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
	int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
	HDC hMemDC;
	HBITMAP hBmp;
	HBITMAP hOldBmp;
	POINT ptOldOrigin;

	// Create new bitmap-back device context, large as the dirty rectangle.
	hMemDC = CreateCompatibleDC(pPaintStruct->hdc);
	hBmp = CreateCompatibleBitmap(pPaintStruct->hdc, cx, cy);
	hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

	// Do the painting into the memory bitmap.
	OffsetViewportOrgEx(hMemDC, -(pPaintStruct->rcPaint.left),
		-(pPaintStruct->rcPaint.top), &ptOldOrigin);

	PaintShapes(hMemDC, &pPaintStruct->rcPaint);
	//PaintShapes(pData, hMemDC, &pPaintStruct->rcPaint, TRUE);

	SetViewportOrgEx(hMemDC, ptOldOrigin.x, ptOldOrigin.y, NULL);

	// Blit the bitmap into the screen. This is really fast operation and although
	// the CustomPaint() can be complex and slow there will be no flicker any more.
	BitBlt(pPaintStruct->hdc, pPaintStruct->rcPaint.left, pPaintStruct->rcPaint.top,
		cx, cy, hMemDC, 0, 0, SRCCOPY);

	// Clean up.
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
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
	shapes[cur] = shapes[cur-1]->copy();

	::InvalidateRect(hWnd, nullptr, true);
}

void DrawController::OnMouseMove() const
{
	inputProcessor->OnMouseMove();
	if (inputProcessor->isPressed()) {
		HDC hdc = GetDC(hWnd);
		DrawRubberBand(hdc);
		ReleaseDC(hWnd, hdc);
	}
}

void DrawController::PaintShapes(const HDC &hdc, RECT* rc) const
{
	FillRect(hdc, rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	for (int i = 0; i < cur; ++i) {
		if (shapes[i] && i != iMarkedShape) {
			shapes[i]->Render(hdc);
		}
	}
	if (iMarkedShape >= 0 && iMarkedShape </*=*/ cur) 
	{
		auto render_data = shapes[iMarkedShape]->GetRenderData();
		HBRUSH hbr = render_data.shouldFill ? CreateSolidBrush(render_data.fillCol) : (HBRUSH)GetStockObject(NULL_BRUSH);// (HBRUSH)GetStockObject(BLACK_BRUSH);
		HPEN hpen = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HBRUSH hbrOld = (HBRUSH)SelectObject(hdc, hbr);
		HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);
		shapes[iMarkedShape]->RenderSimple(hdc);
		SelectObject(hdc, hbrOld);
		SelectObject(hdc, hpenOld);
	}
}

void DrawController::OnPaint() const
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hWnd, &ps);
	PaintDoubleBuffer(&ps);
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

void DrawController::mark(int iShape)
{
	//this->marked_shapes.insert(shapes.at(iShape));
	this->iMarkedShape = iShape;
}

void DrawController::unmark(int iShape)
{
	//this->marked_shapes.erase(shapes.at(iShape));
	this->iMarkedShape = -1;
}


void DrawController::DrawRubberBand(const HDC &hdc) const
{
	MPoint start = inputProcessor->start(),
		end = inputProcessor->end(),
		prev = inputProcessor->prev(),
		prev_start = inputProcessor->start();
	if (einfo.im == IM_CENTERCORNER) {
		start = MReflectPt(start, end);
		prev_start = MReflectPt(inputProcessor->start(), prev);
	}

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

}


// Setters: 
void DrawController::SetInputMethod(InputMethod inputMethod)
{
	einfo.im = inputMethod;
	inputProcessor->setInputMethod(einfo.im);
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