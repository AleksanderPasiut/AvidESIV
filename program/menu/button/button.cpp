#include "button.h"

GRAPHICS* BUTTON::graphics = 0;
ID2D1SolidColorBrush* BUTTON::brush_main = 0;
ID2D1SolidColorBrush* BUTTON::brush_shader_1 = 0;
ID2D1SolidColorBrush* BUTTON::brush_shader_2 = 0;
IDWriteTextFormat* BUTTON::font = 0;

float BUTTON::stroke_width = 1.5f;
float BUTTON::top_margin = 10;
float BUTTON::bottom_margin = 20;
float BUTTON::right_margin = 10;
float BUTTON::sysmenu_size = 25;
float BUTTON::sysmenu_space = 35;
float BUTTON::side_margin = 10;
float BUTTON::np_vsize = 80;
float BUTTON::np_hsize = 18;
float BUTTON::i_vsize = 24;
float BUTTON::i_hsize = 240;

void BUTTON::SharedPainting() const noexcept
{
	switch(state)
	{
		case BS_HOVER: graphics->Target()->FillRectangle(rect, brush_shader_1); break;
		case BS_CLICK: graphics->Target()->FillRectangle(rect, brush_shader_2); break;
	}

	graphics->Target()->DrawRectangle(rect, brush_main, stroke_width);
}
bool BUTTON::Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	bool ret = false;
	float px = static_cast<float>(LOWORD(lParam));
	float py = static_cast<float>(HIWORD(lParam));

	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (state == BS_HOVER)
			{
				state = BS_CLICK;
				graphics->Redraw();
				ret = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (rect.left <= px && px <= rect.right && rect.top <= py && py <= rect.bottom)
			{
				if (state == BS_NONE && !wParam)
				{
					state = BS_HOVER;
					graphics->Redraw();
					ret = true;
				}
			}
			else
			{
				if (state == BS_HOVER)
				{
					state = BS_NONE;
					graphics->Redraw();
					ret = true;
				}
			}

			break;
		}
		case WM_LBUTTONUP:
		{
			if (rect.left <= px && px <= rect.right && rect.top <= py && py <= rect.bottom)
			{
				switch(state)
				{
					case BS_CLICK:
					{
						state = BS_HOVER;
						graphics->Redraw();
						ret = true;
						PostMessage(graphics->Target()->GetHwnd(), WM_COMMAND, 0, static_cast<LPARAM>(type));
						break;
					}
					case BS_NONE:
					{
						state = BS_HOVER;
						graphics->Redraw();
						ret = false;
						break;
					}
				}
			}
			else
			{
				state = BS_NONE;
				graphics->Redraw();
				ret = false;
			}
			break;
		}
	}

	return ret;
}