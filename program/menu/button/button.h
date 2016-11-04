#pragma once

#include "..\\..\\graphics\\graphics.h"

enum BUTTON_TYPE
{
	BT_CLOSE = 0,
	BT_MINIMIZE = 1,
	BT_SETTINGS = 2,
	BT_PREVIOUS = 3,
	BT_NEXT = 4,
	BT_INFO = 5,
};
enum BUTTON_STATE
{
	BS_NONE,
	BS_HOVER,
	BS_CLICK
};

class BUTTON
{
protected:
	static GRAPHICS* graphics;
	static ID2D1SolidColorBrush* brush_main;
	static ID2D1SolidColorBrush* brush_shader_1;
	static ID2D1SolidColorBrush* brush_shader_2;
	static IDWriteTextFormat* font;

	static float stroke_width;
	static float top_margin;
	static float bottom_margin;
	static float right_margin;
	static float sysmenu_size;
	static float sysmenu_space;
	static float side_margin;
	static float np_vsize;
	static float np_hsize;
	static float i_vsize;
	static float i_hsize;

	BUTTON_TYPE type;
	BUTTON_STATE state;
	D2D1_RECT_F rect;

	BUTTON() : state(BS_NONE) {}
	BUTTON(const BUTTON&) = delete;
	BUTTON& operator= (const BUTTON&) = delete;

	void SharedPainting() const noexcept;
	bool Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	friend class MENU;
};

class BUTTON_CLOSE : BUTTON
{
	BUTTON_CLOSE();
	void Paint() const noexcept;

	friend class MENU;
};
class BUTTON_MINIMIZE : BUTTON
{
	BUTTON_MINIMIZE();
	void Paint() const noexcept;

	friend class MENU;
};
class BUTTON_SETTINGS : BUTTON
{
	BUTTON_SETTINGS();
	void Paint() const noexcept;

	friend class MENU;
};
class BUTTON_PREVIOUS : BUTTON
{
	BUTTON_PREVIOUS();
	void Paint() const noexcept;

	friend class MENU;
};
class BUTTON_NEXT : BUTTON
{
	BUTTON_NEXT();
	void Paint() const noexcept;

	friend class MENU;
};
class BUTTON_INFO : BUTTON
{
	BUTTON_INFO();
	void Paint() const noexcept;

	friend class MENU;
};