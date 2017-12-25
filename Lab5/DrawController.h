#pragma once
#include "Helpers.h"
#include "InputProcessor.h"
#include "all_shapes.h"
#include "CustomTable.h"
#include <vector>
#include <map>
struct EditorInfo {
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

	const int shapes_size_step= 255;
	std::vector<Shape*> shapes;
	int cur = 0;
	Shape* current() const;
	bool reallocate();

	void DrawRubberBand() const;
public:
	DrawController(HWND hWnd);
	~DrawController();

	int GetShapesCount() const;
	CustomTableData* exportData() const;

	void Start(Shape*);
	void Start(Shape*, InputMethod, COLORREF outline, COLORREF fill, bool shouldFill);

	void SetInputMethod(InputMethod inputMethod);
	void SetOutlineColor(COLORREF color);
	void SetFillColor(COLORREF color, bool shouldFill);

	void OnMouseDown() const;
	void OnMouseUp();
	void OnMouseMove() const;
	void OnPaint() const;

	void Undo();
};
