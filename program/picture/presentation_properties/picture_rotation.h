#pragma once

#include <Windows.h>
#include "..\\..\\graphics\\graphics.h"

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

	ROTATION();

	void ChangeMarkerType(unsigned new_type) noexcept;

	friend class PRESENTATION_PROPERTIES;
};