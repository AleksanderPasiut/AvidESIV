#include <Windowsx.h>
#include <math.h>

#include "presentation_properties.h"

const float PI = 3.141529f;

PRESENTATION_PROPERTIES::PRESENTATION_PROPERTIES(GRAPHICS* graphics) :
	graphics(graphics),
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
	rotation_marker_type(2)
{
	UpdateMatrix();
}
void PRESENTATION_PROPERTIES::UpdateMatrix() noexcept
{
	ret = D2D1::Matrix3x2F::Scale(D2D1::SizeF(zoom, zoom)) *
		  D2D1::Matrix3x2F::Translation(D2D1::SizeF(static_cast<float>(GetSystemMetrics(SM_CXSCREEN))/2+shift_x,
													static_cast<float>(GetSystemMetrics(SM_CYSCREEN))/2+shift_y));

	matrix = D2D1::Matrix3x2F::Rotation(rotation*180.0f/PI) * ret;

	ret.Invert();

	ret2 = matrix;
	ret2.Invert();
}
void PRESENTATION_PROPERTIES::ResetPosition(const D2D1_RECT_F& rect) noexcept
{
	rotation = 0;
	shift_x = 0;
	shift_y = 0;

	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;

	float cxscreen = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	float cyscreen = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	if (cxscreen < 2*margin + width || cyscreen < 2*margin + height)
		zoom = min((cxscreen - 2*margin)/width, (cyscreen - 2*margin)/height);
	else zoom = 1;

	UpdateMatrix();
}

void PRESENTATION_PROPERTIES::Zooming(WPARAM wParam, LPARAM lParam) noexcept
{
	short delta = GET_WHEEL_DELTA_WPARAM(wParam);
	D2D1_POINT_2F ps = { static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)) };
	D2D1_POINT_2F pl = { ret._11*ps.x + ret._21*ps.y + ret._31, 
						 ret._12*ps.x + ret._22*ps.y + ret._32 };

	float dzoom;
	if (delta > 0)
		dzoom = zoom_speed;
	else dzoom = 1/zoom_speed;

	zoom *= dzoom;

	shift_x += pl.x*(zoom / dzoom - zoom);
	shift_y += pl.y*(zoom / dzoom - zoom);

	UpdateMatrix();
	graphics->Redraw();
}

void PRESENTATION_PROPERTIES::MovingStart(WPARAM wParam, LPARAM lParam) noexcept
{
	Moving.active = true;
	Moving.click = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	Moving.initial_shift_x = shift_x;
	Moving.initial_shift_y = shift_y;
}
void PRESENTATION_PROPERTIES::MovingPerform(WPARAM wParam, LPARAM lParam) noexcept
{
	if (Moving.active)
	{
		shift_x = Moving.initial_shift_x + static_cast<float>(GET_X_LPARAM(lParam) - Moving.click.x);
		shift_y = Moving.initial_shift_y + static_cast<float>(GET_Y_LPARAM(lParam) - Moving.click.y);

		UpdateMatrix();
		graphics->Redraw();
	}
}
void PRESENTATION_PROPERTIES::MovingEnd(WPARAM wParam, LPARAM lParam) noexcept
{
	Moving.active = false;
}

void PRESENTATION_PROPERTIES::RotationStart(WPARAM wParam, LPARAM lParam) noexcept
{
	Rotation.active = true;
	Rotation.ps = { static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)) };
	Rotation.pivot = { ret2._11*Rotation.ps.x + ret2._21*Rotation.ps.y + ret2._31, 
					   ret2._12*Rotation.ps.x + ret2._22*Rotation.ps.y + ret2._32 };
	Rotation.initial_rotation = rotation;
	Rotation.initial_shift = { shift_x, shift_y };
	Rotation.psc = { static_cast<float>(GET_X_LPARAM(lParam)) + Rotation.banned_circle_radius, static_cast<float>(GET_Y_LPARAM(lParam)) };
	SetCursorPos(GET_X_LPARAM(lParam)+static_cast<int>(Rotation.banned_circle_radius), GET_Y_LPARAM(lParam));

	graphics->Redraw();
}
void PRESENTATION_PROPERTIES::RotationPerform(WPARAM wParam, LPARAM lParam) noexcept
{
	if (Rotation.active)
	{
		Rotation.psc = { static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)) };

		float dx = Rotation.psc.x - Rotation.ps.x;
		float dy = Rotation.psc.y - Rotation.ps.y;

		float gxy = static_cast<float>(sqrt(dx*dx + dy*dy));

		if (gxy > Rotation.banned_circle_radius + Rotation.marker_circle_radius)
		{
			float drotation = static_cast<float>(asin(dy / gxy));

			if (dx < 0)
				drotation = PI - drotation;

			rotation = Rotation.initial_rotation + drotation;

			float d_cos = cosf(Rotation.initial_rotation) - cosf(rotation);
			float d_sin = sinf(Rotation.initial_rotation) - sinf(rotation);

			shift_x = Rotation.initial_shift.x + zoom * (d_cos * Rotation.pivot.x - d_sin * Rotation.pivot.y);
			shift_y = Rotation.initial_shift.y + zoom * (d_sin * Rotation.pivot.x + d_cos * Rotation.pivot.y);

			UpdateMatrix();
			graphics->Redraw();
		}
		
	}
}
void PRESENTATION_PROPERTIES::RotationEnd(WPARAM wParam, LPARAM lParam) noexcept
{
	Rotation.active = false;
	graphics->Redraw();
}
void PRESENTATION_PROPERTIES::RotationPaintMarker(ID2D1SolidColorBrush* brush) const noexcept
{
	if (Rotation.active)
	{
		graphics->Target()->DrawEllipse(D2D1::Ellipse(Rotation.ps,
													  Rotation.banned_circle_radius, 
													  Rotation.banned_circle_radius), brush, interface_stroke_width);

		D2D1_POINT_2F marker_circle_point =
		{
			Rotation.ps.x + Rotation.banned_circle_radius*cosf((rotation - Rotation.initial_rotation)),
			Rotation.ps.y + Rotation.banned_circle_radius*sinf((rotation - Rotation.initial_rotation))
		};
		graphics->Target()->DrawEllipse(D2D1::Ellipse(marker_circle_point, Rotation.marker_circle_radius, Rotation.marker_circle_radius), brush, interface_stroke_width);

		D2D1_POINT_2F track_line_start_point =
		{
			Rotation.ps.x + (Rotation.banned_circle_radius+Rotation.marker_circle_radius)*cosf((rotation - Rotation.initial_rotation)),
			Rotation.ps.y + (Rotation.banned_circle_radius+Rotation.marker_circle_radius)*sinf((rotation - Rotation.initial_rotation))
		};
		graphics->Target()->DrawLine(track_line_start_point, Rotation.psc, brush, interface_stroke_width);

		for (float alpha = 0; alpha <= 3; alpha++)
		{
			graphics->Target()->DrawLine(D2D1::Point2F(Rotation.ps.x+Rotation.strokes_inner_radius*cosf(alpha*PI/2),
													   Rotation.ps.y+Rotation.strokes_inner_radius*sinf(alpha*PI/2)),
										 D2D1::Point2F(Rotation.ps.x+Rotation.strokes_outer_radius_90deg*cosf(alpha*PI/2),
													   Rotation.ps.y+Rotation.strokes_outer_radius_90deg*sinf(alpha*PI/2)),
										 brush, interface_stroke_width);

			for (float beta = 1; beta <= 5; beta++)
			{
				graphics->Target()->DrawLine(D2D1::Point2F(Rotation.ps.x+Rotation.strokes_inner_radius*cosf((alpha/2+beta/12)*PI),
														   Rotation.ps.y+Rotation.strokes_inner_radius*sinf((alpha/2+beta/12)*PI)),
											 D2D1::Point2F(Rotation.ps.x+Rotation.strokes_outer_radius_15deg*cosf((alpha/2+beta/12)*PI),
														   Rotation.ps.y+Rotation.strokes_outer_radius_15deg*sinf((alpha/2+beta/12)*PI)),
											 brush, interface_stroke_width);
			}
		}
	}
}
