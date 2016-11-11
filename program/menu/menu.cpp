#include "menu.h"

D2D1_COLOR_F avgColor(const D2D1_COLOR_F& main, const D2D1_COLOR_F& back, float factor, float alpha) noexcept
{
	return D2D1::ColorF(main.r*factor + back.r*(1-factor),
						main.g*factor + back.g*(1-factor),
						main.b*factor + back.b*(1-factor), alpha);
}

void MENU::InitBrushes()
{
	HRESULT hres;

	float shader_1 = 0.4f;
	float shader_2 = 0.7f;
	D2D1_COLOR_F main = D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.8f);

	hres = graphics->Target()->CreateSolidColorBrush(main, &BUTTON::brush_main);

	if (FAILED(hres))
		throw 0;

	try
	{
		hres = graphics->Target()->CreateSolidColorBrush(avgColor(main, graphics->ClearColor(), shader_1, main.a), &BUTTON::brush_shader_1);
		
		if (FAILED(hres))
			throw 0;

		try
		{
			hres = graphics->Target()->CreateSolidColorBrush(avgColor(main, graphics->ClearColor(), shader_2, main.a), &BUTTON::brush_shader_2);

			if (FAILED(hres))
				throw 0;

			try
			{
				hres = graphics->DWriteFactory()->CreateTextFormat(L"Tahoma",
																   0,
																   DWRITE_FONT_WEIGHT_NORMAL,
																   DWRITE_FONT_STYLE_NORMAL,
																   DWRITE_FONT_STRETCH_NORMAL,
																   13.0f,
																   L"pl-pl",
																   &BUTTON::font);
				if (FAILED(hres))
					throw 0;

				BUTTON::font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				BUTTON::font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				BUTTON::font->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
				
				DWRITE_TRIMMING trimming = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
				BUTTON::font->SetTrimming(&trimming, 0);
			}
			catch(...) { BUTTON::brush_shader_2->Release(); throw; }
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
	BUTTON::font->Release();
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
	button_info.Paint();
}
bool MENU::Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	bool bret = false;

	bret |= button_close.Mouse(uMsg, wParam, lParam);
	bret |= button_minimize.Mouse(uMsg, wParam, lParam);
	bret |= button_settings.Mouse(uMsg, wParam, lParam);
	bret |= button_previous.Mouse(uMsg, wParam, lParam);
	bret |= button_next.Mouse(uMsg, wParam, lParam);
	bret |= button_info.Mouse(uMsg, wParam, lParam);

	return bret;
}
void MENU::SetColorInterface(D2D1_COLOR_F main) noexcept
{
	float shader_1 = 0.4f;
	float shader_2 = 0.7f;

	ID2D1SolidColorBrush* tmp_main;
	if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(main, &tmp_main)))
	{
		ID2D1SolidColorBrush* tmp_shader_1;
		if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(avgColor(main, graphics->ClearColor(), shader_1, main.a), &tmp_shader_1)))
		{
			ID2D1SolidColorBrush* tmp_shader_2;
			if (SUCCEEDED(graphics->Target()->CreateSolidColorBrush(avgColor(main, graphics->ClearColor(), shader_2, main.a), &tmp_shader_2)))
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