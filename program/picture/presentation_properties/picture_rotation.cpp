#include "picture_rotation.h"

ROTATION::ROTATION() :
	active(false),
	banned_circle_radius(30),
	marker_circle_radius(8),
	strokes_inner_radius(40),
	strokes_outer_radius_90deg(70),
	strokes_outer_radius_15deg(52) {}
void ROTATION::ChangeMarkerType(unsigned type) noexcept
{
	switch(type)
	{
		case 0:
		{
			banned_circle_radius = 20;
			marker_circle_radius = 4;
			strokes_inner_radius = 25;
			strokes_outer_radius_90deg = 40;
			strokes_outer_radius_15deg = 30;
			break;
		}
		case 1:
		{
			banned_circle_radius = 25;
			marker_circle_radius = 5;
			strokes_inner_radius = 30;
			strokes_outer_radius_90deg = 55;
			strokes_outer_radius_15deg = 38;
			break;
		}
		case 2:
		{
			banned_circle_radius = 30;
			marker_circle_radius = 8;
			strokes_inner_radius = 40;
			strokes_outer_radius_90deg = 70;
			strokes_outer_radius_15deg = 52;
			break;
		}
	}
}
