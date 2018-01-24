#include "stdafx.h"
#include "XDrawController.h"
#include "XGdiGlobal.h"

#define shapes_size_step 255

XGdiDrawController::XGdiDrawController(XGdiContext& ctx)// : hWnd(hWnd)
{
	inputProcessor = new XTwoPointInputProcessor(ctx);
}

XGdiDrawController::~XGdiDrawController()
{
}


void XGdiDrawController::Start(XShape* sh, InputMethod im, XGdiColor* outline, XGdiColor* fill)
{
	if (cur >= shapes.size()) {
		reallocate();
	}
	sh->SetColors(outline, fill);
	shapes[cur] = sh;// dynamic_cast<XMultipointShape*>(sh);
	SetInputMethod(im);
	SetOutlineColor(outline);
	SetFillColor(fill);
}

XShape* XGdiDrawController::current() const
{
	return shapes[cur];
}

bool XGdiDrawController::reallocate() {
	//if (cur < shapes_size) return false;
	shapes.resize(shapes.size() + shapes_size_step);
	return true;
}

void XGdiDrawController::PaintDoubleBuffer(XGdiContext& ctx, PAINTSTRUCT* pPaintStruct) const

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
	
	//XGdiContext gCtx;
	PaintShapes(XGdiContext(ctx.hWnd, hMemDC), &pPaintStruct->rcPaint);

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

void XGdiDrawController::OnMouseDown(XGdiContext& ctx)
{
	if(inputProcessor->OnMouseDown(ctx))
	{
		XBasicShapeFactory factory;
		XPathShape* sh = factory.CreatePath(inputProcessor->get_points(), true);
		sh->SetColors(einfo.outline_color, einfo.fill_color);
		//shapes.emplace_back((XShape*)sh);
		shapes[cur++] = sh;
	};
}

void XGdiDrawController::OnMouseUp(XGdiContext& ctx)
{
	auto pathShape = dynamic_cast<XPathShape*>(shapes[cur]);
	if (pathShape)
	{
		pathShape->SetPoints(inputProcessor->get_points());
	}
	if (inputProcessor->OnMouseUp(ctx))
	{
		XBasicShapeFactory factory;
		XPathShape* sh = factory.CreatePath(inputProcessor->get_points(), true);
		sh->SetColors(einfo.outline_color, einfo.fill_color);
		//shapes.emplace_back((XShape*)sh);
		shapes[cur++] = sh;
		shapes[cur] = sh->clone();
		//shapes[cur]
	};
	inputProcessor->OnMouseUp(ctx);
	//if (!shapes[cur]) return;
	//if (pathShape) {
	//	pathShape->Points() = { inputProcessor->start(), inputProcessor->end() };
	//}
	//else {
	//	shapes[cur]->Move(inputProcessor->end());
	//}
	//shapes[cur]->SetColors(einfo.outline_color, einfo.fill_color);

	//cur++;
	//if (cur >= shapes.size())
	//	reallocate();
	//shapes[cur] = shapes[cur - 1]->clone();

	::InvalidateRect(ctx.hWnd, nullptr, true);
}

void XGdiDrawController::OnMouseMove(XGdiContext& ctx) 
{
	inputProcessor->OnMouseMove(ctx);
	if (inputProcessor->isPressed()) {
		XGdiContext dcContext = ctx.CreateDCContext();
		DrawRubberBand(dcContext);
		dcContext.ReleaseHDC();
	}
}

void XGdiDrawController::PaintShapes(XGdiContext& ctx, RECT* rc) const
{
	FillRect(ctx.hdc, rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	for (int i = 0; i <= cur; ++i) {
		if (shapes[i] && i != iMarkedShape) {
			shapes[i]->Render(ctx);
		}
	}
	if (iMarkedShape >= 0 && iMarkedShape </*=*/ cur)
	{
		XColor *col_fill, *col_out;
		shapes[iMarkedShape]->GetColors(col_out, col_fill);
		XGdiColor *gdi_fill = dynamic_cast<XGdiColor*>(col_fill);
		XGdiColor *gdi_out = dynamic_cast<XGdiColor*>(col_out);
		if (!(gdi_fill || gdi_out)) {
			// TODO: handle error
			return;
		}
		//auto render_data = shapes[iMarkedShape]->GetRenderData();
		//HBRUSH hbr = render_data.shouldFill ? CreateSolidBrush(render_data.fillCol) : (HBRUSH)GetStockObject(NULL_BRUSH);// (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH hbr = gdi_fill->CreateBrush();
		HPEN hpen = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HBRUSH hbrOld = (HBRUSH)SelectObject(ctx.hdc, hbr);
		HPEN hpenOld = (HPEN)SelectObject(ctx.hdc, hpen);
		shapes[iMarkedShape]->RenderSimple(ctx);
		SelectObject(ctx.hdc, hbrOld);
		SelectObject(ctx.hdc, hpenOld);
		DeleteObject(hbr);
		DeleteObject(hpen);
	}
}

void XGdiDrawController::OnPaint(XGdiContext& ctx) 
{
	PAINTSTRUCT ps;
	ctx.hdc = ::BeginPaint(ctx.hWnd, &ps);
	PaintDoubleBuffer(ctx, &ps);
	::EndPaint(ctx.hWnd, &ps);
	ctx.hdc = 0;
}

void XGdiDrawController::Undo(XGdiContext& ctx)
{
	if (cur>0)
	{
		cur--;
		::InvalidateRect(ctx.hWnd, nullptr, true);
	}
}

//void XGdiDrawController::mark(int iShape)
//{
//	//this->marked_shapes.insert(shapes.at(iShape));
//	this->iMarkedShape = iShape;
//}
//
//void XGdiDrawController::unmark(int iShape)
//{
//	//this->marked_shapes.erase(shapes.at(iShape));
//	this->iMarkedShape = -1;
//}


void XGdiDrawController::DrawRubberBand(XGdiContext& ctx) const
{
	//MPoint start = inputProcessor->start(),
	//	end = inputProcessor->end(),
	//	prev = inputProcessor->prev(),
	//	prev_start = inputProcessor->start();
	//if (einfo.im == IM_CENTERCORNER) {
	//	start = MReflectPt(start, end);
	//	prev_start = MReflectPt(inputProcessor->start(), prev);
	//}

	::SetROP2(ctx.hdc, R2_NOTXORPEN);
	::SelectObject(ctx.hdc, ::CreatePen(PS_DOT, 1, 0));
	std::vector<MPoint> pts;
	auto curr = dynamic_cast<XPathShape*>(shapes[cur]);
	if (curr)
	{
		auto pts = inputProcessor->get_points(2);
		auto ptsOld = inputProcessor->get_prev_points(2);
		if (pts.size() > 1 && ptsOld.size() > 1) {
			XImplStorage::inst()->GetImpl<XPathShapeImpl>()->RenderLine(ctx, ptsOld[0], ptsOld[1]);
			XImplStorage::inst()->GetImpl<XPathShapeImpl>()->RenderLine(ctx, pts[0], pts[1]);
		}
	}

}

// Setters: 
void XGdiDrawController::SetInputMethod(InputMethod inputMethod)
{
	einfo.im = inputMethod;
	//inputProcessor->setInputMethod(einfo.im);
}

void XGdiDrawController::SetOutlineColor(XGdiColor* color)
{
	this->einfo.outline_color = color;
}

void XGdiDrawController::SetFillColor(XGdiColor* color)
{
	this->einfo.fill_color = color;
}

int XGdiDrawController::GetShapesCount()
{
	return cur;
}
