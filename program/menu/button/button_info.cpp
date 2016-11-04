#include "button.h"

BUTTON_INFO::BUTTON_INFO()
{
	type = BT_INFO;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	rect = D2D1::RectF((cxscreen - BUTTON::i_hsize)/2,
					   cyscreen - BUTTON::bottom_margin - BUTTON::i_vsize,
					   (cxscreen + BUTTON::i_hsize)/2,
					   cyscreen - BUTTON::bottom_margin);
}

void BUTTON_INFO::Paint() const noexcept
{
	SharedPainting();

}