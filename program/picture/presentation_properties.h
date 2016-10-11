#pragma once

#include "..\\graphics\\graphics.h"

class PRESENTATION_PROPERTIES
{
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

	PRESENTATION_PROPERTIES() : 
		margin(50),
		zoom(1.0f),
		zoom_speed(1.1f),
		shift_x(0.0f),
		shift_y(0.0f),
		rotation(0.0f),
		color_background(D2D1::ColorF(0.1f, 0.2f, 0.1f)),
		color_interface(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.8f)),
		interpolation_mode(1),
		interface_stroke_width(1.5f),
		rotation_marker_type(2) { UpdateMatrix(); }

	void UpdateMatrix() noexcept;

	friend class PICTURE;
	friend class LOADER;
};