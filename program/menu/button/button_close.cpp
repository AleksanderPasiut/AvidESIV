#include "button.h"

BUTTON_CLOSE::BUTTON_CLOSE()
{
	type = BT_CLOSE;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF(cxscreen - BUTTON::right_margin - BUTTON::sysmenu_size,
					   BUTTON::top_margin,
					   cxscreen - BUTTON::right_margin,
					   BUTTON::top_margin + BUTTON::sysmenu_size);
}

void BUTTON_CLOSE::Paint() const noexcept
{
	SharedPainting();

	graphics->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											   0.2f*rect.bottom + 0.8f*rect.top), 
								 D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											   0.8f*rect.bottom + 0.2f*rect.top),
								 brush_main, BUTTON::stroke_width);

	graphics->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											   0.8f*rect.bottom + 0.2f*rect.top), 
								 D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											   0.2f*rect.bottom + 0.8f*rect.top),
								 brush_main, BUTTON::stroke_width);
}