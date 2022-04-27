#pragma once
#include <Windows.h>

//This is the Actual Window and the Game Loop but the Game Loop could be offset into a constant buffer timer or something similar
class Window
{
public:
	Window();

	bool isRun();

	RECT getClientWindowRect();
	RECT getSizeScreen();

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();

	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};