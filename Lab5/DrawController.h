#pragma once
#include "Helpers.h"
#include "InputProcessor.h"
struct EditorInfo {
	ShapeType st;
	InputMethod im;
	COLORREF outlineCol;
	COLORREF fillCol;
	bool shouldFill;
};
class DrawController
{
	HWND hWnd;
	EditorInfo einfo;
	TwoPointInputProcessor *inputProcessor;
	void DrawRubberBand() const;
public:
	DrawController(HWND hWnd);
	~DrawController();

	EditorInfo GetEditorInfo();

	void SetEditorInfo(EditorInfo einfo);
	void SetShapeType(ShapeType shapeType);
	void SetInputMethod(InputMethod inputMethod);
	void SetOutlineColor(COLORREF color);
	void SetFillColor(COLORREF color, bool shouldFill);

	void OnMouseDown() const;
	void OnMouseUp() const;
	void OnMouseMove() const;
	void OnPaint() const;

	void Undo();
};

