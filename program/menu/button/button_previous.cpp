#include "button.h"

BUTTON_PREVIOUS::BUTTON_PREVIOUS()
{
	type = BT_PREVIOUS;

	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF(BUTTON::side_margin ,
					   (cyscreen - BUTTON::np_vsize)/2,
					   BUTTON::side_margin + BUTTON::np_hsize,
					   (cyscreen + BUTTON::np_vsize)/2);
}

void BUTTON_PREVIOUS::Paint() const noexcept
{
	SharedPainting();

	D2D1_POINT_2F top = D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, (rect.top + rect.bottom - BUTTON::sysmenu_size)/2);
	D2D1_POINT_2F middle = D2D1::Point2F(0.8f*rect.left + 0.2f*rect.right, (rect.top + rect.bottom)/2);
	D2D1_POINT_2F bottom = D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, (rect.top + rect.bottom + BUTTON::sysmenu_size)/2);

	graphics->Target()->DrawLine(top, middle, brush_main);
	graphics->Target()->DrawLine(bottom, middle, brush_main);
	graphics->Target()->DrawLine(top, bottom, brush_main);
}