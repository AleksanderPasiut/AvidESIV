#include "picture.h"
#include "show_info\\show_info.h"

const wchar_t* PICTURE::PathToFirstPicture() const noexcept
{
	wchar_t* command = GetCommandLineW();

	int argc;

	wchar_t** argv = CommandLineToArgvW(command, &argc);

	if (argc < 2)
		return 0;
	else return argv[1];
}

PICTURE::PICTURE(GRAPHICS* graphics) : graphics(graphics), pp(graphics), loader(0)
{
	HRESULT hres;
	
	graphics->SetClearColor(pp.color_background);
	
	hres = graphics->Target()->CreateSolidColorBrush(pp.color_interface, &brush);

	if (FAILED(hres))
		throw 0;

	try
	{
		loader = new LOADER(graphics);
	}
	catch(...) { brush->Release(); throw; }
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
	pp.RotationPaintMarker(brush);
}
bool PICTURE::Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
		{
			pp.MovingPerform(wParam, lParam);
			pp.RotationPerform(wParam, lParam);
			break;
		}
		case WM_LBUTTONDOWN: pp.MovingStart(wParam, lParam); break;
		case WM_LBUTTONUP: pp.MovingEnd(wParam, lParam); break;
		case WM_RBUTTONDOWN: pp.RotationStart(wParam, lParam); break;
		case WM_RBUTTONUP: pp.RotationEnd(wParam, lParam); break;
		case WM_MOUSEWHEEL: pp.Zooming(wParam, lParam); break;
		case WM_LBUTTONDBLCLK:
		{
			pp.ResetPosition(loader->rect); 
			graphics->Redraw();
			break;
		}
	}
	return true;
}
void PICTURE::FirstLoad() noexcept
{
	try
	{
		loader->FirstLoad(PathToFirstPicture());
		pp.ResetPosition(loader->rect);
	}
	catch(int) { MessageBoxW(graphics->Target()->GetHwnd(), L"B³¹d odczytu pliku lub niew³aœciwy format", L"B³¹d", MB_OK); }
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
		pp.ResetPosition(loader->rect);

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);

	return ret;
}
void PICTURE::Previous() noexcept
{
	if (!loader->LoadPrevious(&pp))
		pp.ResetPosition(loader->rect);
}
void PICTURE::Next() noexcept
{
	if (!loader->LoadNext(&pp))
		pp.ResetPosition(loader->rect);
}
void PICTURE::ShowInfo() const noexcept
{
	return; // functionality is cancelled for now
	/*SHOW_INFO_DATA sid;
	loader->FillShowInfoData(&sid);
	DialogBoxParam(0, "dialog_show_info", graphics->Target()->GetHwnd(), reinterpret_cast<DLGPROC>(DialogShowWindowProc), reinterpret_cast<LPARAM>(&sid));*/
}