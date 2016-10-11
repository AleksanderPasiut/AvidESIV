#pragma once

#include <Windows.h>

class MOVING
{
	POINT click;
	float initial_shift_x;
	float initial_shift_y;
	bool active;

	MOVING() : active(false) {}

	friend class PICTURE;
};