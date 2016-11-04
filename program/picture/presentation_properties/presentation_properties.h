#pragma once

#include "..\\..\\graphics\\graphics.h"
#include "picture_moving.h"
#include "picture_rotation.h"

class PRESENTATION_PROPERTIES
{
	GRAPHICS* graphics;

	D2D1::Matrix3x2F matrix;
	D2D1::Matrix3x2F ret;
	D2D1::Matrix3x2F ret2;
	float margin;
	float zoom;
	float zoom_speed;
	float shift_x;
	float shift_y;
	float rotation;

	D2D1_COLOR_F color_background;
	D2D1_COLOR_F color_interface;

	unsigned interpolation_mode;
	float interface_stroke_width;
	unsigned rotation_marker_type;

	MOVING Moving;
	ROTATION Rotation;

	PRESENTATION_PROPERTIES(GRAPHICS* graphics);
	void UpdateMatrix() noexcept;
	void ResetPosition(const D2D1_RECT_F&) noexcept;

	void Zooming(WPARAM, LPARAM) noexcept;

	void MovingStart(WPARAM, LPARAM) noexcept;
	void MovingPerform(WPARAM, LPARAM) noexcept;
	void MovingEnd(WPARAM, LPARAM) noexcept;

	void RotationStart(WPARAM, LPARAM) noexcept;
	void RotationPerform(WPARAM, LPARAM) noexcept;
	void RotationEnd(WPARAM, LPARAM) noexcept;
	void RotationPaintMarker(ID2D1SolidColorBrush*) const noexcept;

	void ChangeMarkerType(unsigned mode) noexcept { Rotation.ChangeMarkerType(mode); }
	friend class PICTURE;
	friend class LOADER;
};