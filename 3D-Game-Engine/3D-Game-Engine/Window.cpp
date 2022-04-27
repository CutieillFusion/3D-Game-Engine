#include "Window.h"
#include <exception>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// Event fired when the window is created
		break;
	}
	case WM_SIZE:
	{
		// Event fired when the window is resized
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
		{
			window->onSize();
		}
		break;
	}
	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
		{
			window->onFocus();
		}
		break;
	}
	case WM_KILLFOCUS:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		::PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

Window::Window()
{
	//Setting up WNDCLASSEX object
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	//Failure Check
	if (!::RegisterClassEx(&wc))
	{
		throw std::exception("Window not successfully created");
	}

	//Creates the Window
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"3D Game Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if (!m_hwnd)
	{
		throw std::exception("Window not successfully created");
	}

	//Show the Window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	//Set this flag to true to indicate that the Window is Initialized and Running
	m_is_run = true;

}



//This is the Update Loop
bool Window::broadcast()
{
	MSG msg;

	if (!this->m_is_init)//Initializes the Window
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//Need to give some Time between Updates
	Sleep(1);

	return true;
}

bool Window::isRun()
{
	if (m_is_run)
	{
		broadcast();
	}
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

RECT Window::getSizeScreen()
{
	RECT rc;

	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	
	return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onSize()
{
}

Window::~Window()
{
}