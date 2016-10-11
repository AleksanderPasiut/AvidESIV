#include "graphics.h"

GRAPHICS::GRAPHICS(HWND hwnd)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_d2d1)))
		throw 0;

	try
	{
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&factory_dwrite))))
			throw 0;

		try
		{
			if (FAILED(factory_d2d1->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
															D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))),
															&target)))
					   throw 0;
		}
		catch(...) { factory_dwrite->Release(); throw; }
	}
	catch(...) { factory_d2d1->Release(); throw; }
}
GRAPHICS::~GRAPHICS() noexcept
{
	target->Release();
	factory_dwrite->Release();
	factory_d2d1->Release();
}

void GRAPHICS::BeginDraw() noexcept
{
	target->BeginDraw();
	target->Clear(clear_color);
}
void GRAPHICS::EndDraw() noexcept
{
	target->EndDraw();
}
void GRAPHICS::Redraw() noexcept
{
	RedrawWindow(target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
}