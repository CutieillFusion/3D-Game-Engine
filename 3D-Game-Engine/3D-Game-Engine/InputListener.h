#pragma once
#include "Point.h"

//Listens to Input
class InputListener
{
public:
	InputListener()
	{

	}

	~InputListener()
	{

	}

	//Keyboard pure virtual callback functions
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	//Mose pure virtual callback function
	virtual void onMouseMove(const Point& mouse_pos) = 0;

	virtual void onLeftMouseDown(const Point& mouse_pos) = 0;
	virtual void onLeftMouseUp(const Point& mouse_pos) = 0;

	virtual void onRightMouseDown(const Point& mouse_pos) = 0;
	virtual void onRightMouseUp(const Point& mouse_pos) = 0;
};