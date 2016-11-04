#pragma once

#include <Windows.h>

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

class GRAPHICS
{
	ID2D1Factory* factory_d2d1;
	IDWriteFactory* factory_dwrite;

	ID2D1HwndRenderTarget* target;

	D2D1_COLOR_F clear_color;

public:
	GRAPHICS(HWND);
	GRAPHICS(const GRAPHICS&) = delete;
	GRAPHICS& operator= (const GRAPHICS&) = delete;
	~GRAPHICS() noexcept;

	void BeginDraw() noexcept;
	void EndDraw() noexcept;
	void Redraw() noexcept;
	
	IDWriteFactory* DWriteFactory() noexcept { return factory_dwrite; }
	ID2D1HwndRenderTarget* Target() noexcept { return target; }
	D2D1_COLOR_F ClearColor() noexcept { return clear_color; }

	void SetClearColor(D2D1_COLOR_F color) noexcept { clear_color = color; }
};