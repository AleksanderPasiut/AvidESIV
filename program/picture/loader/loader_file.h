#pragma once

class LOADER_FILE
{
	const wchar_t* url;
	wchar_t* name;
	bool opened;
	float zoom;
	float shift_x;
	float shift_y;
	float rotation;

	LOADER_FILE() : url(0), opened(false), zoom(1), shift_x(0), shift_y(0), rotation(0) {}
	LOADER_FILE(const LOADER_FILE&) = delete;
	LOADER_FILE& operator= (const LOADER_FILE&) = delete;
	~LOADER_FILE() noexcept { delete[] url; }

	friend class LOADER;
};