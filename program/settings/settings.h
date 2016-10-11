#pragma once

#include <Windows.h>

#include "..\\menu\\menu.h"
#include "..\\picture\\picture.h"

#define CTRL_GB_INTERPOLATION 0x101
#define CTRL_GB_ROTATION_MARKER 0x102
#define CTRL_GB_SET_PROGRAM_COLORS 0x103
#define CTRL_GB_MOUSEWHEEL_SENSITIVITY 0x104
#define CTRL_RB_INTERPOLATION_LINEAR 0x201
#define CTRL_RB_INTERPOLATION_NEAREST_NEIGHBOR 0x202
#define CTRL_RB_ROTATION_MARKER_SMALL 0x203
#define CTRL_RB_ROTATION_MARKER_MEDIUM 0x204
#define CTRL_RB_ROTATION_MARKER_BIG 0x205
#define CTRL_PB_SET_BACKGROUND_COLOR 0x206
#define CTRL_PB_SET_INTERFACE_COLOR 0x207
#define CTRL_RB_MOUSEWHEEL_SENSITIVITY_SMALL 0x208
#define CTRL_RB_MOUSEWHEEL_SENSITIVITY_BIG 0x209
#define CTRL_OK 0x301
#define CTRL_CANCEL 0x302

struct SETTINGS_SET
{
	unsigned interpolation_type;
	unsigned rotation_marker_type;
	unsigned mousewheel_sensitivity;
	D2D1_COLOR_F background_color;
	D2D1_COLOR_F interface_color;
};

class SETTINGS
{
	static SETTINGS* own;
	HWND hwnd_parent;
	MENU* menu;
	PICTURE* picture;
	wchar_t* path;

	SETTINGS_SET set;

	static COLORREF cust_colors[16];

	SETTINGS(HWND, MENU*, PICTURE*);
	SETTINGS(const SETTINGS&) = delete;
	SETTINGS& operator= (const SETTINGS&) = delete;
	~SETTINGS() noexcept;

	void InitSavingPath();

	void Proceed() noexcept;
	bool LoadSet() noexcept;
	void RetrieveSet() noexcept;
	void ApplySet() const noexcept;
	void SaveSet() const noexcept;

	static void InitDialogValues(HWND) noexcept;
	static void ChangeInterpolation(HWND, unsigned) noexcept;
	static void ChangeRotationMarker(HWND, unsigned) noexcept;
	static void ChangeMousewheelSensitivity(HWND, unsigned) noexcept;
	static void ChangeColorBackground(HWND) noexcept;
	static void ChangeColorInterface(HWND) noexcept;

	friend class PROGRAM;
	friend BOOL CALLBACK DialogSettingsProc(HWND, UINT, WPARAM, LPARAM);
};

BOOL CALLBACK DialogSettingsProc(HWND, UINT, WPARAM, LPARAM);