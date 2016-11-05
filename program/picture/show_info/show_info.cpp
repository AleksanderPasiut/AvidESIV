#include "show_info.h"

void InitDialog(HWND hwnd, LPARAM lParam) noexcept
{
	SHOW_INFO_DATA& sid = *reinterpret_cast<SHOW_INFO_DATA*>(lParam);

	SetWindowTextW(hwnd, sid.name);
}

BOOL CALLBACK DialogShowWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			InitDialog(hwnd, lParam);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_OK:
				{
					EndDialog(hwnd,0);
					return TRUE;
				}
			}
			break;
		}
	}
	return FALSE;
};