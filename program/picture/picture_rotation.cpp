#include <math.h>

#include "picture.h"

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


void PICTURE::RotationStart(WPARAM wParam, LPARAM lParam) noexcept
{
	rotation.active = true;
	rotation.ps = { static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) };
	rotation.pivot = { pp.ret2._11*rotation.ps.x + pp.ret2._21*rotation.ps.y + pp.ret2._31, 
					   pp.ret2._12*rotation.ps.x + pp.ret2._22*rotation.ps.y + pp.ret2._32 };
	rotation.initial_rotation = pp.rotation;
	rotation.initial_shift = { pp.shift_x, pp.shift_y };
	rotation.psc = { static_cast<float>(LOWORD(lParam)) + rotation.banned_circle_radius, static_cast<float>(HIWORD(lParam)) };
	SetCursorPos(LOWORD(lParam)+static_cast<int>(rotation.banned_circle_radius), HIWORD(lParam));

	graphics->Redraw();
}
void PICTURE::RotationPerform(WPARAM wParam, LPARAM lParam) noexcept
{
	if (rotation.active)
	{
		rotation.psc = { static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) };

		float dx = rotation.psc.x - rotation.ps.x;
		float dy = rotation.psc.y - rotation.ps.y;

		float gxy = static_cast<float>(sqrt(dx*dx + dy*dy));

		if (gxy > rotation.banned_circle_radius + rotation.marker_circle_radius)
		{
			float drotation = static_cast<float>(asin(dy / gxy));

			if (dx < 0)
				drotation = 3.141529f - drotation;

			pp.rotation = rotation.initial_rotation + drotation;

			float d_cos = cos(rotation.initial_rotation) - cos(pp.rotation);
			float d_sin = sin(rotation.initial_rotation) - sin(pp.rotation);

			pp.shift_x = rotation.initial_shift.x + pp.zoom * (d_cos * rotation.pivot.x - d_sin * rotation.pivot.y);
			pp.shift_y = rotation.initial_shift.y + pp.zoom * (d_sin * rotation.pivot.x + d_cos * rotation.pivot.y);

			pp.UpdateMatrix();
			graphics->Redraw();
		}
		
	}
}
void PICTURE::RotationEnd(WPARAM wParam, LPARAM lParam) noexcept
{
	rotation.active = false;
	graphics->Redraw();
}
void PICTURE::RotationPaintMarker() const noexcept
{
	if (rotation.active)
	{
		graphics->Target()->DrawEllipse(D2D1::Ellipse(rotation.ps,
													  rotation.banned_circle_radius, 
													  rotation.banned_circle_radius), brush, pp.interface_stroke_width);

		D2D1_POINT_2F marker_circle_point =
		{
			rotation.ps.x + rotation.banned_circle_radius*cos((pp.rotation - rotation.initial_rotation)),
			rotation.ps.y + rotation.banned_circle_radius*sin((pp.rotation - rotation.initial_rotation))
		};
		graphics->Target()->DrawEllipse(D2D1::Ellipse(marker_circle_point, rotation.marker_circle_radius, rotation.marker_circle_radius), brush, pp.interface_stroke_width);

		D2D1_POINT_2F track_line_start_point =
		{
			rotation.ps.x + (rotation.banned_circle_radius+rotation.marker_circle_radius)*cos((pp.rotation - rotation.initial_rotation)),
			rotation.ps.y + (rotation.banned_circle_radius+rotation.marker_circle_radius)*sin((pp.rotation - rotation.initial_rotation))
		};
		graphics->Target()->DrawLine(track_line_start_point, rotation.psc, brush, pp.interface_stroke_width);

		for (float alpha = 0; alpha <= 3; alpha++)
		{
			graphics->Target()->DrawLine(D2D1::Point2F(rotation.ps.x+rotation.strokes_inner_radius*cos(alpha*3.141529f/2),
													   rotation.ps.y+rotation.strokes_inner_radius*sin(alpha*3.141529f/2)),
										 D2D1::Point2F(rotation.ps.x+rotation.strokes_outer_radius_90deg*cos(alpha*3.141529f/2),
													   rotation.ps.y+rotation.strokes_outer_radius_90deg*sin(alpha*3.141529f/2)),
										 brush, pp.interface_stroke_width);

			for (float beta = 1; beta <= 5; beta++)
			{
				graphics->Target()->DrawLine(D2D1::Point2F(rotation.ps.x+rotation.strokes_inner_radius*cos((alpha/2+beta/12)*3.141529f),
														   rotation.ps.y+rotation.strokes_inner_radius*sin((alpha/2+beta/12)*3.141529f)),
											 D2D1::Point2F(rotation.ps.x+rotation.strokes_outer_radius_15deg*cos((alpha/2+beta/12)*3.141529f),
														   rotation.ps.y+rotation.strokes_outer_radius_15deg*sin((alpha/2+beta/12)*3.141529f)),
											 brush, pp.interface_stroke_width);
			}
		}
	}
}