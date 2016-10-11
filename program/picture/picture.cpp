#include "picture.h"

const wchar_t* PICTURE::PathToFirstPicture() const noexcept
{
	wchar_t* command = GetCommandLineW();

	int argc;

	wchar_t** argv = CommandLineToArgvW(command, &argc);

	if (argc < 2)
		return 0;
	else return argv[1];
}
void PICTURE::ResetPosition() noexcept
{
	pp.rotation = 0;
	pp.shift_x = 0;
	pp.shift_y = 0;

	float width = loader->rect.right - loader->rect.left;
	float height = loader->rect.bottom - loader->rect.top;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	if (cxscreen < 2*pp.margin + width || cyscreen < 2*pp.margin + height)
	{
		pp.zoom = min((cxscreen - 2*pp.margin)/width,
					  (cyscreen - 2*pp.margin)/height);
	}
	else pp.zoom = 1;

	pp.UpdateMatrix();
}

PICTURE::PICTURE(GRAPHICS* graphics) : graphics(graphics)
{
	HRESULT hres;
	
	graphics->SetClearColor(pp.color_background);
	
	hres = graphics->Target()->CreateSolidColorBrush(pp.color_interface, &brush);

	if (FAILED(hres))
		throw 0;

	try
	{
		loader = new LOADER(graphics);

		try
		{
			loader->FirstLoad(PathToFirstPicture());
		}
		catch(int) { MessageBoxW(graphics->Target()->GetHwnd(), L"B³¹d odczytu pliku lub niew³aœciwy format", L"B³¹d", MB_OK); }
		catch(...) { delete loader; throw; }
	}
	catch(...) { brush->Release(); }
	ResetPosition();
}
PICTURE::~PICTURE() noexcept
{
	brush->Release();
	delete loader;
}

void PICTURE::Paint() const noexcept
{
	if (loader->bitmap)
	{
		graphics->Target()->SetTransform(pp.matrix);
	
		graphics->Target()->DrawBitmap(loader->bitmap,
									   loader->rect,
									   1.0f,
									   static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(pp.interpolation_mode));
	}
	
	graphics->Target()->SetTransform(D2D1::IdentityMatrix());
	RotationPaintMarker();
}
bool PICTURE::Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
		{
			MovingPerform(wParam, lParam);
			RotationPerform(wParam, lParam);
			break;
		}
		case WM_LBUTTONDOWN: MovingStart(wParam, lParam); break;
		case WM_LBUTTONUP: MovingEnd(wParam, lParam); break;
		case WM_RBUTTONDOWN: RotationStart(wParam, lParam); break;
		case WM_RBUTTONUP: RotationEnd(wParam, lParam); break;
		case WM_MOUSEWHEEL: Zooming(wParam, lParam); break;
		case WM_LBUTTONDBLCLK:
		{
			ResetPosition(); 
			graphics->Redraw();
			break;
		}
	}
	return true;
}
bool PICTURE::External(WPARAM wParam) noexcept
{
	bool ret = false;

	HANDLE hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, L"AvidESIVFileShared");

	if (!hMapFile)
		return false;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, static_cast<int>(wParam)*sizeof(wchar_t));

	if (!buffer)
		return false;

	wchar_t* command = reinterpret_cast<wchar_t*>(buffer);
	int argc;
	wchar_t** argv = CommandLineToArgvW(command, &argc);

	if (argc < 2)
		return false;

	if (ret = loader->LoadExternal(argv[1]))
		ResetPosition();

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);

	return ret;
}
void PICTURE::Previous() noexcept
{
	if (!loader->LoadPrevious(&pp))
		ResetPosition();
}
void PICTURE::Next() noexcept
{
	if (!loader->LoadNext(&pp))
		ResetPosition();
}