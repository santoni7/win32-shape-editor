#pragma once
class XContext {
public:	XContext();
		virtual ~XContext() = 0;
};
class XColor {
public: XColor();
		virtual ~XColor();
};
enum InputMethod { IM_CENTERCORNER, IM_CORNERCORNER };


struct MPoint {
	int x;
	int y;
	MPoint(int x = 0, int y = 0) : x(x), y(y) { }
};

MPoint operator+(const MPoint& lpt, const MPoint& rpt);
MPoint operator-(const MPoint& pt);
MPoint operator-(const MPoint& lpt, const MPoint& rpt);


int MReflectPt(int center, int point);
MPoint MReflectPt(const MPoint& center, const MPoint& point);
double MLineLength(const MPoint& p1, const MPoint& p2);


enum MouseButtonState { Pressed, Released };
struct MouseState
{
	MouseButtonState LButton = Released, 
		RButton = Released;
	MPoint ScreenCursorPos = { 0,0 }, 
		ClientCursorPos = { 0,0 };
	MouseState() { }
	MouseState(MouseButtonState lb, MouseButtonState rb, MPoint screen, MPoint client); //TODO 24.01: Implement ctor 
};