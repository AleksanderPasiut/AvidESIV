#include "loader.h"

void LOADER::LoadCore(const wchar_t* url)
{
	if (!url)
		return;

	HRESULT hres;

	ID2D1Bitmap* tmp_bmp = 0;

	IWICBitmapDecoder* decoder;
	hres = factory->CreateDecoderFromFilename(url,
											  NULL,
											  GENERIC_READ,
											  WICDecodeMetadataCacheOnLoad,
											  &decoder);
	if (SUCCEEDED(hres))
	{
		IWICBitmapFrameDecode* frame;
		hres = decoder->GetFrame(0, &frame);

		if (SUCCEEDED(hres))
		{
			unsigned width, height;
			hres = frame->GetSize(&width, &height);

			if (SUCCEEDED(hres))
			{
				rect = D2D1::RectF(-static_cast<float>(width)/2, 
								   -static_cast<float>(height)/2,
								   static_cast<float>(width)/2,
								   static_cast<float>(height)/2);

				IWICFormatConverter* converter;
				hres = factory->CreateFormatConverter(&converter);

				if (SUCCEEDED(hres))
				{
					UINT32 mbs = graphics->Target()->GetMaximumBitmapSize();

					if (width <= mbs && height <= mbs)
					{
						hres = converter->Initialize(frame,
													 GUID_WICPixelFormat32bppPBGRA,
													 WICBitmapDitherTypeNone,
													 0,
													 0,
													 WICBitmapPaletteTypeMedianCut);
					}
					else
					{
						IWICBitmapScaler* scaler;
						hres = factory->CreateBitmapScaler(&scaler);

						if (SUCCEEDED(hres))
						{	
							UINT dwidth, dheight;

							if (width >= height)
							{
								dwidth = mbs;
								dheight = static_cast<UINT>(static_cast<float>(mbs)/static_cast<float>(width) * height);
							}
							else
							{
								dheight = mbs;
								dwidth = static_cast<UINT>(static_cast<float>(mbs)/static_cast<float>(height) * width);
							}

							hres = scaler->Initialize(frame, dwidth, dheight, WICBitmapInterpolationModeLinear);

							if (SUCCEEDED(hres))
							{
								hres = converter->Initialize(scaler,
															 GUID_WICPixelFormat32bppPBGRA,
															 WICBitmapDitherTypeNone,
															 0,
															 0,
															 WICBitmapPaletteTypeMedianCut);
							}
						}
						scaler->Release();
					}

					if (SUCCEEDED(hres))
						hres = graphics->Target()->CreateBitmapFromWicBitmap(converter, 0, &tmp_bmp);

					converter->Release();
				}
			}
			frame->Release();
		}
		decoder->Release();
	}

	if (SUCCEEDED(hres))
	{
		if (bitmap)
			bitmap->Release();

		bitmap = tmp_bmp;
	}
	else throw 0;
}