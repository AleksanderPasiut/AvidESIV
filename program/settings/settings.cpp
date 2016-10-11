#include <fstream>

#include "settings.h"

SETTINGS* SETTINGS::own = 0;
COLORREF SETTINGS::cust_colors[16];

SETTINGS::SETTINGS(HWND hwnd_parent, MENU* menu, PICTURE* picture) : 
	hwnd_parent(hwnd_parent),
	menu(menu),
	picture(picture)
{
	InitSavingPath();

	if (own)
		throw 0;
	else own = this;

	if (LoadSet())
		ApplySet();
}
SETTINGS::~SETTINGS() noexcept
{
	RetrieveSet();
	SaveSet();
	delete[] path;
}

void SETTINGS::InitSavingPath()
{
	wchar_t* tmp = GetCommandLineW();
	int argc;
	wchar_t** argv = CommandLineToArgvW(tmp, &argc);

	unsigned size = lstrlenW(argv[0]);
	path = new wchar_t [size+10];
	memcpy(path, argv[0], size);
	for (unsigned i = size; i > 0; i--)
		if (path[i] == L'\\')
		{
			memcpy(path+i, L"config.cfg", 11*sizeof(wchar_t));
			break;
		}
}

void SETTINGS::Proceed() noexcept
{
	RetrieveSet();

	if (DialogBox(0, "dialog_settings", hwnd_parent, reinterpret_cast<DLGPROC>(DialogSettingsProc)))
		ApplySet();
}
bool SETTINGS::LoadSet() noexcept
{
	using namespace std;

	fstream FS(path, fstream::in);

	if (!FS)
		return false;

	FS.read(reinterpret_cast<char*>(&set), sizeof(SETTINGS_SET));

	FS.close();

	return true;
}
void SETTINGS::RetrieveSet() noexcept
{
	set.interface_color = picture->RetColorInterface();
	set.background_color = picture->RetColorBackground();
	set.interpolation_type = picture->RetInterpolationMode();
	set.rotation_marker_type = picture->RetMarkerType();
	set.mousewheel_sensitivity = picture->RetZoomSpeed();
}
void SETTINGS::ApplySet() const noexcept
{
	menu->SetColorInterface(set.interface_color);
	picture->SetColorInterface(set.interface_color);
	picture->SetColorBackground(set.background_color);
	picture->SetZoomSpeed(set.mousewheel_sensitivity);
	picture->SetMarkerType(set.rotation_marker_type);
	picture->SetInterpolationMode(set.interpolation_type);
}
void SETTINGS::SaveSet() const noexcept
{
	using namespace std;

	fstream FS(path, fstream::out);

	if (!FS)
		return;

	FS.write(reinterpret_cast<const char*>(&set), sizeof(SETTINGS_SET));

	FS.close();
}

void SETTINGS::InitDialogValues(HWND hwnd) noexcept
{
	CheckDlgButton(hwnd, CTRL_RB_INTERPOLATION_LINEAR, own->set.interpolation_type == D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	CheckDlgButton(hwnd, CTRL_RB_INTERPOLATION_NEAREST_NEIGHBOR, own->set.interpolation_type == D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_SMALL, own->set.rotation_marker_type == 0);
	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_MEDIUM, own->set.rotation_marker_type == 1);
	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_BIG, own->set.rotation_marker_type == 2);

	CheckDlgButton(hwnd, CTRL_RB_MOUSEWHEEL_SENSITIVITY_SMALL, own->set.mousewheel_sensitivity == 0);
	CheckDlgButton(hwnd, CTRL_RB_MOUSEWHEEL_SENSITIVITY_BIG, own->set.mousewheel_sensitivity == 1);
}
void SETTINGS::ChangeInterpolation(HWND hwnd, unsigned arg) noexcept
{
	switch(arg)
	{
		case CTRL_RB_INTERPOLATION_LINEAR: own->set.interpolation_type = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR; break;
		case CTRL_RB_INTERPOLATION_NEAREST_NEIGHBOR: own->set.interpolation_type = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR; break;
	}

	CheckDlgButton(hwnd, CTRL_RB_INTERPOLATION_LINEAR, own->set.interpolation_type == D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	CheckDlgButton(hwnd, CTRL_RB_INTERPOLATION_NEAREST_NEIGHBOR, own->set.interpolation_type == D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}
void SETTINGS::ChangeRotationMarker(HWND hwnd, unsigned arg) noexcept
{
	switch(arg)
	{
		case CTRL_RB_ROTATION_MARKER_SMALL: own->set.rotation_marker_type = 0; break;
		case CTRL_RB_ROTATION_MARKER_MEDIUM: own->set.rotation_marker_type = 1; break;
		case CTRL_RB_ROTATION_MARKER_BIG: own->set.rotation_marker_type = 2; break;
	}

	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_SMALL, own->set.rotation_marker_type == 0);
	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_MEDIUM, own->set.rotation_marker_type == 1);
	CheckDlgButton(hwnd, CTRL_RB_ROTATION_MARKER_BIG, own->set.rotation_marker_type == 2);
}
void SETTINGS::ChangeMousewheelSensitivity(HWND hwnd, unsigned arg) noexcept
{
	switch(arg)
	{
		case CTRL_RB_MOUSEWHEEL_SENSITIVITY_SMALL: own->set.mousewheel_sensitivity = 0; break;
		case CTRL_RB_MOUSEWHEEL_SENSITIVITY_BIG: own->set.mousewheel_sensitivity = 1; break;
	}

	CheckDlgButton(hwnd, CTRL_RB_MOUSEWHEEL_SENSITIVITY_SMALL, own->set.mousewheel_sensitivity == 0);
	CheckDlgButton(hwnd, CTRL_RB_MOUSEWHEEL_SENSITIVITY_BIG, own->set.mousewheel_sensitivity == 1);
}
void SETTINGS::ChangeColorBackground(HWND hwnd) noexcept
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = cust_colors;
	cc.rgbResult = RGB(static_cast<unsigned>(255*own->set.background_color.r),
					   static_cast<unsigned>(255*own->set.background_color.g),
					   static_cast<unsigned>(255*own->set.background_color.b));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if(ChooseColor(&cc))
	{
		own->set.background_color = D2D1::ColorF(static_cast<float>(GetRValue(cc.rgbResult))/255,
												 static_cast<float>(GetGValue(cc.rgbResult))/255,
												 static_cast<float>(GetBValue(cc.rgbResult))/255);
	}
}
void SETTINGS::ChangeColorInterface(HWND hwnd) noexcept
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = cust_colors;
	cc.rgbResult = RGB(static_cast<unsigned>(255*own->set.interface_color.r),
					   static_cast<unsigned>(255*own->set.interface_color.g),
					   static_cast<unsigned>(255*own->set.interface_color.b));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if(ChooseColor(&cc))
	{
		own->set.interface_color = D2D1::ColorF(static_cast<float>(GetRValue(cc.rgbResult))/255,
												static_cast<float>(GetGValue(cc.rgbResult))/255,
												static_cast<float>(GetBValue(cc.rgbResult))/255);
	}
}

BOOL CALLBACK DialogSettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			SETTINGS::InitDialogValues(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_RB_INTERPOLATION_LINEAR:
				case CTRL_RB_INTERPOLATION_NEAREST_NEIGHBOR:
				{
					SETTINGS::ChangeInterpolation(hwnd, LOWORD(wParam));
					break;
				}
				case CTRL_RB_ROTATION_MARKER_BIG:
				case CTRL_RB_ROTATION_MARKER_MEDIUM:
				case CTRL_RB_ROTATION_MARKER_SMALL:
				{
					SETTINGS::ChangeRotationMarker(hwnd, LOWORD(wParam));
					break;
				}
				case CTRL_RB_MOUSEWHEEL_SENSITIVITY_BIG:
				case CTRL_RB_MOUSEWHEEL_SENSITIVITY_SMALL:
				{
					SETTINGS::ChangeMousewheelSensitivity(hwnd, LOWORD(wParam));
					break;
				}
				case CTRL_PB_SET_BACKGROUND_COLOR:
				{
					SETTINGS::ChangeColorBackground(hwnd);
					break;
				}
				case CTRL_PB_SET_INTERFACE_COLOR:
				{
					SETTINGS::ChangeColorInterface(hwnd);
					break;
				}
				case CTRL_OK:
				{
					EndDialog(hwnd, 1);
					return TRUE;
				}
				case CTRL_CANCEL:
				{
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
			break;
		}
	}
	return FALSE;
}