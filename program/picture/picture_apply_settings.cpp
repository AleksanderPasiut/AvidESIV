#include "picture.h"

void PICTURE::SetInterpolationMode(unsigned mode) noexcept
{
	if (mode > 1)
		return;

	pp.interpolation_mode = mode;
}
void PICTURE::SetMarkerType(unsigned mode) noexcept
{
	if (mode > 2)
		return;

	pp.rotation_marker_type = mode;
	pp.ChangeMarkerType(mode);
}
void PICTURE::SetColorBackground(D2D1_COLOR_F color) noexcept
{
	pp.color_background = color;
	graphics->SetClearColor(color);
}
void PICTURE::SetColorInterface(D2D1_COLOR_F color) noexcept
{
	ID2D1SolidColorBrush* tmp;

	if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(color, &tmp)))
	{
		brush->Release();
		brush = tmp;
	}
}
void PICTURE::SetZoomSpeed(unsigned arg) noexcept
{
	switch(arg)
	{
		case 0: pp.zoom_speed = 1.05f; break;
		case 1: pp.zoom_speed = 1.1f; break;
	}
}