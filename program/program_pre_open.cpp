#include "program.h"

HANDLE PROGRAM::event = 0;

bool PROGRAM::PreOpen() noexcept
{
	event = CreateEventW(0, false, true, L"Avid ESIV Event");

	if (!GetLastError())
		return false;

	HWND target = FindWindow("AvidESIV_WindowClass", "Avid ESIV");
	wchar_t* command_line = GetCommandLineW();
	int command_line_length = int(wcslen(command_line));
	HANDLE hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, command_line_length*sizeof(wchar_t), L"AvidESIVFileShared");
	
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, command_line_length*sizeof(wchar_t));
	memcpy(buffer, command_line, command_line_length*sizeof(wchar_t));
	SendMessage(target, 0x8001, static_cast<WPARAM>(command_line_length), 0);

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
	return true;
}