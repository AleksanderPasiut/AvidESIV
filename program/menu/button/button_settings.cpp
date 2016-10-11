#include <vector>

#include "button.h"

BUTTON_SETTINGS::BUTTON_SETTINGS()
{
	type = BT_SETTINGS;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF(cxscreen - BUTTON::right_margin - BUTTON::sysmenu_size - 2*BUTTON::sysmenu_space,
					   BUTTON::top_margin,
					   cxscreen - BUTTON::right_margin - 2*BUTTON::sysmenu_space,
					   BUTTON::top_margin + BUTTON::sysmenu_size);
}

void BUTTON_SETTINGS::Paint() const noexcept
{
	SharedPainting();

	D2D1_POINT_2F center = D2D1::Point2F((rect.left + rect.right)/2, (rect.top + rect.bottom)/2);
	float r1 = (rect.right - rect.left)*0.18f;
	float r2 = (rect.right - rect.left)*0.33f;
	float stroke = 2.5f;

	graphics->Target()->DrawEllipse(D2D1::Ellipse(center, r1, r1), brush_main, stroke);

	for (float i = 0; i < 6; i+= 1.0f)
	{
		graphics->Target()->DrawLine(D2D1::Point2F(center.x + r1*sin(i/3*3.141529f),
												   center.y + r1*cos(i/3*3.141529f)),
									 D2D1::Point2F(center.x + r2*sin(i/3*3.141529f),
												   center.y + r2*cos(i/3*3.141529f)),
									 brush_main, stroke);
	}
}