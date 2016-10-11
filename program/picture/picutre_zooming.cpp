#include "picture.h"

void PICTURE::Zooming(WPARAM wParam, LPARAM lParam) noexcept
{
	short delta = GET_WHEEL_DELTA_WPARAM(wParam);
	D2D1_POINT_2F ps = { static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) };
	D2D1_POINT_2F pl = { pp.ret._11*ps.x + pp.ret._21*ps.y + pp.ret._31, 
						 pp.ret._12*ps.x + pp.ret._22*ps.y + pp.ret._32 };

	float dzoom;
	if (delta > 0)
		dzoom = pp.zoom_speed;
	else dzoom = 1/pp.zoom_speed;

	pp.zoom *= dzoom;

	pp.shift_x += pl.x*(pp.zoom / dzoom - pp.zoom);
	pp.shift_y += pl.y*(pp.zoom / dzoom - pp.zoom);

	pp.UpdateMatrix();
	graphics->Redraw();
}
