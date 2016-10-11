#include "window.h"

void WINDOW::InitWindowClass(HINSTANCE hInstance, WNDPROC wndproc)
{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = wndproc;
	wc.lpszClassName = "AvidESIV_WindowClass";
	wc.hInstance = hInstance;
	wc.style = CS_DBLCLKS;
	wc.hIcon = LoadIcon(hInstance, "icon");
	wc.hIconSm = LoadIcon(hInstance, "icon_sm");
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));

	if (!RegisterClassEx(&wc))
		throw 0;
}

void WINDOW::InitHwnd()
{
	hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		"Avid ESIV",
		WS_POPUP,
		0, //CW_USEDEFAULT,
		0, //CW_USEDEFAULT,
		GetSystemMetrics(SM_CXSCREEN), //CW_USEDEFAULT,
		GetSystemMetrics(SM_CYSCREEN), //CW_USEDEFAULT,
		0,
		0,
		wc.hInstance,
		0);

	if (!hwnd)
		throw 0;
}

WINDOW::WINDOW(HINSTANCE hInstance, WNDPROC wndproc)
{
	try
	{
		InitWindowClass(hInstance, wndproc);

		try
		{
			InitHwnd();
		}
		catch(...) { UnregisterClass(wc.lpszClassName, wc.hInstance); throw; }
	}
	catch(...) { throw; }
}

WINDOW::~WINDOW() noexcept
{
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}