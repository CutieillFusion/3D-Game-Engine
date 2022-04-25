#pragma once
#include "InputListener.h"
#include <unordered_set>
#include "Point.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);
public://Singleton Behavior
	static InputSystem* get();
	static void create();
	static void release();
private:
	std::unordered_set<InputListener*> m_set_listeners;//Holds all the InputListeners
	unsigned char m_keys_state[256] = {};//holds Current Keyboard Data
	unsigned char m_old_keys_state[256] = {};//holds Last Update's Keyboard Data
	Point m_old_mouse_pos;//last Update's Mouse Pos
	bool m_first_time = true;
	static InputSystem* m_system;
};