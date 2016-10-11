#include "picture.h"

void PICTURE::MovingStart(WPARAM wParam, LPARAM lParam) noexcept
{
	moving.active = true;
	moving.click = { LOWORD(lParam), HIWORD(lParam) };
	moving.initial_shift_x = pp.shift_x;
	moving.initial_shift_y = pp.shift_y;
}
void PICTURE::MovingPerform(WPARAM wParam, LPARAM lParam) noexcept
{
	if (moving.active)
	{
		pp.shift_x = moving.initial_shift_x + static_cast<float>(LOWORD(lParam) - moving.click.x);
		pp.shift_y = moving.initial_shift_y + static_cast<float>(HIWORD(lParam) - moving.click.y);

		pp.UpdateMatrix();
		graphics->Redraw();
	}
}
void PICTURE::MovingEnd(WPARAM wParam, LPARAM lParam) noexcept
{
	moving.active = false;
}