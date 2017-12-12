#pragma once
#include "Helpers.h"
#include "InputProcessor.h"
#include "all_shapes.h"
#include <vector>
#include <map>
struct EditorInfo {
	//ShapeType st;
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
	std::allocator<Shape> allocator;
	int cur = 0;
	Shape* current() const;
	bool reallocate();

	void DrawRubberBand() const;
public:
	DrawController(HWND hWnd);
	~DrawController();

	int GetShapesCount() const;

	//template <typename T>
	//	typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type
	//void Start(std::allocator<T> allocator);
	void Start(Shape*);
	//void SetShapeType(ShapeType shapeType);
	void SetInputMethod(InputMethod inputMethod);
	void SetOutlineColor(COLORREF color);
	void SetFillColor(COLORREF color, bool shouldFill);

	void OnMouseDown() const;
	void OnMouseUp();
	void OnMouseMove() const;
	void OnPaint() const;

	void Undo();
};

//template <typename T>
//typename std::enable_if<std::is_base_of<Shape, T>::value, T>::type
//void DrawController::Start(std::allocator<T> alloc)
//{
//	this->allocator = alloc;
//	if (cur >= shapes.size()) {
//		reallocate();
//	}
//	shapes[cur] = alloc.allocate(1);
//}
//
