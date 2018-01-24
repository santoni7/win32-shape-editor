#pragma once
#include "XBasicShapes.h"
#include "XInputProcessor.h"
#include "XGdiGlobal.h"
#include <vector>
class XGdiDrawController
{
	//HWND hWnd;
protected:
	std::vector<XShape*> shapes;

	struct EditorInfo {
		InputMethod im;
		XGdiColor *fill_color;
		XGdiColor *outline_color;
	} einfo;
	int cur = 0;
	XGdiTwoPointInputProcessor* inputProcessor;
	XShape* current() const;
	bool reallocate();
	int iMarkedShape = -1;
public:
	XGdiDrawController(XGdiContext&);
	~XGdiDrawController();

	void Start(XShape*, InputMethod, XGdiColor* outline, XGdiColor* fill);

	void Undo(XGdiContext&);

	void OnMouseDown(XGdiContext&);
	void OnMouseUp(XGdiContext&);
	void OnMouseMove(XGdiContext&);
	void OnPaint(XGdiContext&);

	void SetInputMethod(InputMethod inputMethod);
	void SetOutlineColor(XGdiColor* color);
	void SetFillColor(XGdiColor* color);

	int GetShapesCount();
private:
	void PaintDoubleBuffer(XGdiContext& ctx, PAINTSTRUCT* pPaintStruct) const;
	void PaintShapes(XGdiContext& ctx, RECT* rc) const;
	void DrawRubberBand(XGdiContext& hdc) const;
};

