#pragma once

#include "..\\graphics\\graphics.h"
#include "button\\button.h"

class MENU
{
	GRAPHICS* graphics;

	BUTTON_CLOSE button_close;
	BUTTON_MINIMIZE button_minimize;
	BUTTON_SETTINGS button_settings;
	BUTTON_PREVIOUS button_previous;
	BUTTON_NEXT button_next;
	BUTTON_INFO button_info;

	void InitBrushes();

	MENU(GRAPHICS* graphics);
	MENU(const MENU&) = delete;
	MENU& operator= (const MENU&) = delete;
	~MENU() noexcept;

	void Paint() const noexcept;
	bool Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	void SetColorInterface(D2D1_COLOR_F color) noexcept;
	void SetButtonInfoText(wchar_t* text) noexcept { button_info.SetText(text); }

	friend class SETTINGS;
	friend class PROGRAM;
};