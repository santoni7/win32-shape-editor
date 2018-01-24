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


	static DrawController *_instance;
	DrawController(HWND hWnd);
	DrawController();
	DrawController(const DrawController&) = delete;
	DrawController& operator=(const DrawController&) = delete;

	const int shapes_size_step= 255;
	std::vector<Shape*> shapes;
	int iMarkedShape = -1; 
	int cur = 0;
	Shape* current() const;
	bool reallocate();
	void PaintDoubleBuffer(PAINTSTRUCT* pPaintStruct) const;
	void PaintShapes(const HDC &hdc, RECT* rc) const;
	void DrawRubberBand(const HDC &hdc) const;
public:
	static DrawController* instance()
	{
		if (!_instance) _instance = new DrawController();
		return _instance;
	}
	void SetHWND(HWND);
	~DrawController();

	int GetShapesCount() const;
	std::vector<Shape*>& GetShapes();
	void Clear();
	void SetShapes(std::vector<Shape*>, int count);

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
