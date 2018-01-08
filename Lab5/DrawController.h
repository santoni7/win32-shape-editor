#pragma once
#include "Helpers.h"
#include "InputProcessor.h"
#include "all_shapes.h"
#include "CustomTableControl.h"
#include <vector>
#include <map>
#include <set>
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
	//std::set<Shape*> marked_shapes;
	int iMarkedShape = -1; 
	int cur = 0;
	Shape* current() const;
	bool reallocate();
	void PaintDoubleBuffer(PAINTSTRUCT* pPaintStruct) const;
	void PaintShapes(const HDC &hdc, RECT* rc) const;
	void DrawRubberBand(const HDC &hdc) const;
public:
	DrawController(HWND hWnd);
	~DrawController();

	int GetShapesCount() const;
	std::vector<Shape*>& GetShapes();

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
	void mark(int iShape);
	void unmark(int iShape);
};
