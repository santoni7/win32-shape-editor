#pragma once
#include "Helpers.h"
#include "Editor.h"
class ShapeEditorController
{
	HWND hWnd;
	ShapeType shapeType;
	InputMethod inputMethod;
	COLORREF outlineCol;
	COLORREF fillCol;
	bool shouldFill;
	ShapeEditor *editor;
	void DrawRubberBand() const;
public:
	ShapeEditorController(HWND hWnd);
	~ShapeEditorController();

	void SetShapeType(ShapeType shapeType);
	void SetEditor(ShapeEditor* editor);
	void SetInputMethod(InputMethod inputMethod);
	void SetOutlineColor(COLORREF color);
	void SetFillColor(COLORREF color, bool shouldFill);
	void OnMouseDown() const;
	void OnMouseUp() const;
	void OnMouseMove() const;
	void OnPaint() const;

	void Undo();
};

