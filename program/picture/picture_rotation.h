#pragma once

#include <Windows.h>

class ROTATION
{
	D2D1_POINT_2F ps;
	D2D1_POINT_2F psc;
	D2D1_POINT_2F pivot;
	float initial_rotation;
	D2D1_POINT_2F initial_shift;
	bool active;

	float banned_circle_radius;
	float marker_circle_radius;

	float strokes_inner_radius;
	float strokes_outer_radius_90deg;
	float strokes_outer_radius_15deg;

	ROTATION() : 
		active(false),
		banned_circle_radius(30),
		marker_circle_radius(8),
		strokes_inner_radius(40),
		strokes_outer_radius_90deg(70),
		strokes_outer_radius_15deg(52) {}

	void ChangeMarkerType(unsigned new_type) noexcept;

	friend class PICTURE;
};