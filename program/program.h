#pragma once

#include "window\\window.h"
#include "graphics\\graphics.h"
#include "picture\\picture.h"
#include "menu\\menu.h"
#include "settings\\settings.h"

class PROGRAM
{
	static HANDLE event;
	static bool PreOpen() noexcept;

	WINDOW* mainWindow;
	GRAPHICS* graphics;
	PICTURE* picture;
	MENU* menu;
	SETTINGS* settings;

	PROGRAM(HINSTANCE, WNDPROC);
	PROGRAM(const PROGRAM&) = delete;
	PROGRAM& operator= (const PROGRAM&) = delete;
	~PROGRAM() noexcept;

	LRESULT Message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	friend LRESULT CALLBACK EventProc(HWND, UINT, WPARAM, LPARAM);
	friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
};
