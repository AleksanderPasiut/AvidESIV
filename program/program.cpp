#include "program.h"

PROGRAM::PROGRAM(HINSTANCE hInstance, WNDPROC wndproc)
{
	try
	{
		mainWindow = new WINDOW(hInstance, wndproc);

		try
		{
			graphics = new GRAPHICS(mainWindow->RetHwnd());

			try
			{
				picture = new PICTURE(graphics);

				try
				{
					menu = new MENU(graphics);

					try
					{
						settings = new SETTINGS(mainWindow->RetHwnd(), menu, picture);
					}
					catch(...) { delete menu; throw; }
				}
				catch(...) { delete picture; throw; }
			}
			catch(...) { delete graphics; throw; }
		}
		catch(...) { delete mainWindow; throw; }
	}
	catch(...) { throw; }

	ShowWindow(mainWindow->RetHwnd(), SW_SHOWNORMAL);
}

PROGRAM::~PROGRAM() noexcept
{
	delete settings;
	delete menu;
	delete picture;
	delete graphics;
	delete mainWindow;
	CloseHandle(event);
}

LRESULT PROGRAM::Message(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			switch(lParam)
			{
				case BT_CLOSE: PostQuitMessage(0); break;
				case BT_MINIMIZE: ShowWindow(mainWindow->RetHwnd(), SW_MINIMIZE); break;
				case BT_SETTINGS: settings->Proceed(); break;
				case BT_PREVIOUS: picture->Previous(); break;
				case BT_NEXT: picture->Next(); break;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDBLCLK:
		{
			if (menu->Mouse(uMsg, wParam, lParam))
				break;

			picture->Mouse(uMsg, wParam, lParam);
			break;
		}
		case WM_PAINT:
		{
			graphics->BeginDraw();
			picture->Paint();
			menu->Paint();
			graphics->EndDraw();
			break;
		}
		case 0x8001:
		{
			if (picture->External(wParam))
				ShowWindow(mainWindow->RetHwnd(), SW_RESTORE);

			break;
		}
		case WM_CLOSE: PostQuitMessage(0); break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
