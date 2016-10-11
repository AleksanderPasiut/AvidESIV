#include "loader.h"

LOADER::LOADER(GRAPHICS* graphics) : graphics(graphics), bitmap(0)
{
	HRESULT hres;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hres))
		throw 0;

	try
	{
		hres = CoCreateInstance(CLSID_WICImagingFactory1,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IWICImagingFactory,
								reinterpret_cast<void**>(&factory));
		if (FAILED(hres))
			throw 0;
	}
	catch(...) { CoUninitialize(); throw; }
}
LOADER::~LOADER() noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		delete (*it);

	tab.clear();

	if (bitmap)
		bitmap->Release();

	factory->Release();
	CoUninitialize();
}

void LOADER::FirstLoad(const wchar_t* url) 
{
	LoadAllFiles(url);
	LoadCore(url);
}
bool LOADER::LoadNext(PRESENTATION_PROPERTIES* pp) noexcept
{
	if (tab.empty())
		return true;

	bool ret = false;

	(*current)->rotation = pp->rotation;
	(*current)->shift_x = pp->shift_x;
	(*current)->shift_y = pp->shift_y;
	(*current)->zoom = pp->zoom;

	auto backup = current;

	for (;;)
	{
		current++;

		if (current == tab.end())
			current = tab.begin();

		if (current == backup)
			break;

		try
		{
			LoadCore((*current)->url);

			ret = (*current)->opened;
			(*current)->opened = true;

			pp->rotation = (*current)->rotation;
			pp->shift_x = (*current)->shift_x;
			pp->shift_y = (*current)->shift_y;
			pp->zoom = (*current)->zoom;
			pp->UpdateMatrix();
			break;
		}
		catch(...) {}
	}
	
	return ret;
}
bool LOADER::LoadPrevious(PRESENTATION_PROPERTIES* pp) noexcept
{
	if (tab.empty())
		return true;

	bool ret = false;

	(*current)->rotation = pp->rotation;
	(*current)->shift_x = pp->shift_x;
	(*current)->shift_y = pp->shift_y;
	(*current)->zoom = pp->zoom;

	auto backup = current;

	for (;;)
	{
		if (current == tab.begin())
			current = tab.end();
		
		current--;

		if (current == backup)
			break;

		try
		{
			LoadCore((*current)->url);

			ret = (*current)->opened;
			(*current)->opened = true;

			pp->rotation = (*current)->rotation;
			pp->shift_x = (*current)->shift_x;
			pp->shift_y = (*current)->shift_y;
			pp->zoom = (*current)->zoom;
			pp->UpdateMatrix();
			break;
		}
		catch(...) {}
	}

	return ret;
}
bool LOADER::LoadExternal(const wchar_t* url) noexcept
{
	try { LoadCore(url); } catch(...) { return false; }

	for (auto it = tab.begin(); it != tab.end(); it++)
		delete *it;

	tab.clear();

	LoadAllFiles(url);

	return true;
}
