#include "menu.h"

void MENU::InitBrushes()
{
	HRESULT hres;

	float shader_1 = 0.4f;
	float shader_2 = 0.7f;

	hres = graphics->Target()->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.8f), &BUTTON::brush_main);

	if (FAILED(hres))
		throw 0;

	try
	{
		hres = graphics->Target()->CreateSolidColorBrush(D2D1::ColorF(shader_1*0.8f, shader_1*0.8f, shader_1*0.8f, 0.8f), &BUTTON::brush_shader_1);
		
		if (FAILED(hres))
			throw 0;

		try
		{
			hres = graphics->Target()->CreateSolidColorBrush(D2D1::ColorF(shader_2*0.8f, shader_2*0.8f, shader_2*0.8f, 0.8f), &BUTTON::brush_shader_2);

			if (FAILED(hres))
				throw 0;
		}
		catch(...) { BUTTON::brush_shader_1->Release(); throw; }
	}
	catch(...) { BUTTON::brush_main->Release(); throw; }
}

MENU::MENU(GRAPHICS* graphics) : graphics(graphics)
{
	BUTTON::graphics = graphics;

	InitBrushes();
}
MENU::~MENU() noexcept
{
	BUTTON::brush_shader_2->Release();
	BUTTON::brush_shader_1->Release();
	BUTTON::brush_main->Release();
}

void MENU::Paint() const noexcept
{
	graphics->Target()->SetTransform(D2D1::IdentityMatrix());

	button_close.Paint();
	button_minimize.Paint();
	button_settings.Paint();
	button_previous.Paint();
	button_next.Paint();
}
bool MENU::Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	bool bret = false;

	bret |= button_close.Mouse(uMsg, wParam, lParam);
	bret |= button_minimize.Mouse(uMsg, wParam, lParam);
	bret |= button_settings.Mouse(uMsg, wParam, lParam);
	bret |= button_previous.Mouse(uMsg, wParam, lParam);
	bret |= button_next.Mouse(uMsg, wParam, lParam);

	return bret;
}
void MENU::SetColorInterface(D2D1_COLOR_F color) noexcept
{
	float shader_1 = 0.4f;
	float shader_2 = 0.7f;

	ID2D1SolidColorBrush* tmp_main;
	if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(color, &tmp_main)))
	{
		ID2D1SolidColorBrush* tmp_shader_1;
		if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(D2D1::ColorF(shader_1*color.r, shader_1*color.g, shader_1*color.b, 0.8f), &tmp_shader_1)))
		{
			ID2D1SolidColorBrush* tmp_shader_2;
			if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(D2D1::ColorF(shader_2*color.r, shader_2*color.g, shader_2*color.b, 0.8f), &tmp_shader_2)))
			{
				BUTTON::brush_main->Release();
				BUTTON::brush_shader_1->Release();
				BUTTON::brush_shader_2->Release();

				BUTTON::brush_main = tmp_main;
				BUTTON::brush_shader_1 = tmp_shader_1;
				BUTTON::brush_shader_2 = tmp_shader_2;
				return;

			}
			tmp_shader_1->Release();
		}
		tmp_main->Release();
	}
}