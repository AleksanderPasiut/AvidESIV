#pragma once

#include <Windows.h>

#define CTRL_OK 0

struct SHOW_INFO_DATA
{
	wchar_t* full_url;
	wchar_t* name;
	unsigned width;
	unsigned height;
};

BOOL CALLBACK DialogShowWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);