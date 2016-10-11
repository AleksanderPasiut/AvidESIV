#pragma once

#include <Windows.h>

class WINDOW
{
	WNDCLASSEX wc;
	HWND hwnd;

	inline void InitWindowClass(HINSTANCE, WNDPROC);
	inline void InitHwnd();

	WINDOW(HINSTANCE, WNDPROC);
	WINDOW(const WINDOW&) = delete;
	WINDOW& operator= (const WINDOW&) = delete;
	~WINDOW() noexcept;

	HWND RetHwnd() const noexcept { return hwnd; }

	friend class PROGRAM;
};