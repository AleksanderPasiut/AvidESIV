#include "button.h"

BUTTON_MINIMIZE::BUTTON_MINIMIZE()
{
	type = BT_MINIMIZE;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF(cxscreen - BUTTON::right_margin - BUTTON::sysmenu_size - BUTTON::sysmenu_space,
					   BUTTON::top_margin,
					   cxscreen - BUTTON::right_margin - BUTTON::sysmenu_space,
					   BUTTON::top_margin + BUTTON::sysmenu_size);
}

void BUTTON_MINIMIZE::Paint() const noexcept
{
	SharedPainting();

	graphics->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											   0.7f*rect.bottom + 0.3f*rect.top), 
								 D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											   0.7f*rect.bottom + 0.3f*rect.top),
								 brush_main, BUTTON::stroke_width);
}