#include "button.h"

BUTTON_NEXT::BUTTON_NEXT()
{
	type = BT_NEXT;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF(cxscreen - BUTTON::side_margin - BUTTON::np_hsize,
					   (cyscreen - BUTTON::np_vsize)/2,
					   cxscreen - BUTTON::side_margin,
					   (cyscreen + BUTTON::np_vsize)/2);
}

void BUTTON_NEXT::Paint() const noexcept
{
	SharedPainting();

	D2D1_POINT_2F top = D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, (rect.top + rect.bottom - BUTTON::sysmenu_size)/2);
	D2D1_POINT_2F middle = D2D1::Point2F(0.2f*rect.left + 0.8f*rect.right, (rect.top + rect.bottom)/2);
	D2D1_POINT_2F bottom = D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, (rect.top + rect.bottom + BUTTON::sysmenu_size)/2);

	graphics->Target()->DrawLine(top, middle, brush_main);
	graphics->Target()->DrawLine(bottom, middle, brush_main);
	graphics->Target()->DrawLine(top, bottom, brush_main);
}