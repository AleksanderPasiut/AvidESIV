#include "presentation_properties.h"

void PRESENTATION_PROPERTIES::UpdateMatrix() noexcept
{
	ret = D2D1::Matrix3x2F::Scale(D2D1::SizeF(zoom, zoom)) *
		  D2D1::Matrix3x2F::Translation(D2D1::SizeF(static_cast<float>(GetSystemMetrics(SM_CXSCREEN))/2+shift_x,
													static_cast<float>(GetSystemMetrics(SM_CYSCREEN))/2+shift_y));

	matrix = D2D1::Matrix3x2F::Rotation(rotation*180.0f/3.141529f) * ret;

	ret.Invert();

	ret2 = matrix;
	ret2.Invert();
}