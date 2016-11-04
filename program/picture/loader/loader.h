#pragma once

#include <Windows.h>
#include <wincodec.h>
#include <vector>

#include "..\\..\\graphics\\graphics.h"
#include "..\\presentation_properties\presentation_properties.h"
#include "loader_file.h"

#pragma comment(lib,"windowscodecs.lib")

class LOADER
{
	GRAPHICS* graphics;

	IWICImagingFactory* factory;
	ID2D1Bitmap* bitmap;
	D2D1_RECT_F rect;

	std::vector<LOADER_FILE*> tab;
	std::vector<LOADER_FILE*>::iterator current;

	LOADER(GRAPHICS*);
	LOADER(const LOADER&) = delete;
	LOADER& operator= (const LOADER&) = delete;
	~LOADER() noexcept;

	void FirstLoad(const wchar_t* url);
	void LoadAllFiles(const wchar_t* url) noexcept;
	void LoadCore(const wchar_t* url);
	bool LoadNext(PRESENTATION_PROPERTIES*) noexcept;
	bool LoadPrevious(PRESENTATION_PROPERTIES*) noexcept;
	bool LoadExternal(const wchar_t* url) noexcept;

	friend class PICTURE;
};