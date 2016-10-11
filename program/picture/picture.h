#pragma once

#include "..\\graphics\\graphics.h"
#include "loader\\loader.h"
#include "presentation_properties.h"
#include "picture_moving.h"
#include "picture_rotation.h"

class PICTURE
{
	GRAPHICS* graphics;
	PRESENTATION_PROPERTIES pp;
	LOADER* loader;
	ID2D1SolidColorBrush* brush;

	const wchar_t* PathToFirstPicture() const noexcept;
	void ResetPosition() noexcept;
	void Zooming(WPARAM, LPARAM) noexcept;

	MOVING moving;
	void MovingStart(WPARAM, LPARAM) noexcept;
	void MovingPerform(WPARAM, LPARAM) noexcept;
	void MovingEnd(WPARAM, LPARAM) noexcept;

	ROTATION rotation;
	void RotationStart(WPARAM, LPARAM) noexcept;
	void RotationPerform(WPARAM, LPARAM) noexcept;
	void RotationEnd(WPARAM, LPARAM) noexcept;
	void RotationPaintMarker() const noexcept;

	PICTURE(GRAPHICS*);
	PICTURE(const PICTURE&) = delete;
	PICTURE& operator= (const PICTURE&) = delete;
	~PICTURE() noexcept;

	void Paint() const noexcept;
	bool Mouse(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	bool External(WPARAM wParam) noexcept;
	void Previous() noexcept;
	void Next() noexcept;

	unsigned RetInterpolationMode() const noexcept { return pp.interpolation_mode; }
	unsigned RetMarkerType() const noexcept { return pp.rotation_marker_type; }
	D2D1_COLOR_F RetColorBackground() const noexcept { return pp.color_background; }
	D2D1_COLOR_F RetColorInterface() const noexcept { return pp.color_interface; }
	unsigned RetZoomSpeed() const noexcept { return pp.zoom_speed > 1.07f ? 1 : 0; }

	void SetInterpolationMode(unsigned) noexcept;
	void SetMarkerType(unsigned) noexcept;
	void SetColorBackground(D2D1_COLOR_F) noexcept;
	void SetColorInterface(D2D1_COLOR_F) noexcept;
	void SetZoomSpeed(unsigned) noexcept;

	friend class SETTINGS;
	friend class PROGRAM;
};