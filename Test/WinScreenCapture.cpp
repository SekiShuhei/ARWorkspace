
#include <math.h>

#include "WinScreenCapture.hpp"



WinScreenCapture::WinScreenCapture() :
	bmpInfo()
{
	

}


WinScreenCapture::~WinScreenCapture()
{
	DeleteDC(this->hMemDC);
	DeleteObject(this->hBitmap);

}

DWORD WinScreenCapture::GetBitmapImageSize(const BITMAPINFO& bitmap_info) const
{
	
	DWORD lineSizeDW = 0;
	switch (bitmap_info.bmiHeader.biBitCount)
	{
	case 1:
	case 2:
	case 4:
	{
		DWORD pixelsPerDW = 8 * sizeof(DWORD) / bitmap_info.bmiHeader.biBitCount; // DWORD ���̉�f��

		lineSizeDW = ceil(bitmap_info.bmiHeader.biWidth / pixelsPerDW);
		break;
	}
	case 8:
	case 16:
	case 24:
	case 32:
	{
		DWORD bytesPerPixel = bitmap_info.bmiHeader.biBitCount / 8; // �P��f������̃o�C�g��
		lineSizeDW = bytesPerPixel * bitmap_info.bmiHeader.biWidth;
		lineSizeDW = ceil(lineSizeDW / sizeof(DWORD));
		break;
	}
	default:
		break;
	}
	DWORD lineSize = lineSizeDW * sizeof(DWORD);
	return lineSize * bitmap_info.bmiHeader.biHeight;

}


bool WinScreenCapture::CaptureScreen(s3d::Image& read_image, int x, int y, int width, int height)
{
	
	this->capture_rect.left = x + ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	this->capture_rect.top = y + ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	this->capture_rect.right = x + width;
	this->capture_rect.bottom = y + height;

	this->bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->bmpInfo.bmiHeader.biWidth = width;
	this->bmpInfo.bmiHeader.biHeight = height;
	this->bmpInfo.bmiHeader.biPlanes = 1;
	this->bmpInfo.bmiHeader.biBitCount = 32;
	this->bmpInfo.bmiHeader.biCompression = BI_RGB;
	this->bmpInfo.bmiHeader.biSizeImage = this->GetBitmapImageSize(this->bmpInfo);

	HDC screen_dc = GetDC(NULL);
	
	HDC hPrevMemDC = this->hMemDC;
	HBITMAP hPrevBitmap = this->hBitmap;
	this->hBitmap = ::CreateDIBSection(screen_dc, &this->bmpInfo, DIB_RGB_COLORS, (void**)&this->lpPixel, NULL, 0);
	bool error = true;
	if (this->hBitmap != NULL)
	{	
		this->hMemDC = ::CreateCompatibleDC(screen_dc);
		if (this->hMemDC != NULL)
		{
			::SelectObject(this->hMemDC, this->hBitmap);
			error = false;
		}
	} else {
		//...
	}
	if (hPrevBitmap != NULL)
	{
		::DeleteObject(hPrevBitmap);  //BMP���폜�������AlpPixel�������I�ɉ�������
	}
	if (hPrevMemDC != NULL)
	{
		::DeleteDC(hPrevMemDC);
	}
	if (error)
	{
		return false;
	}
	bool result = false;

	::BitBlt(this->hMemDC, 0, 0, this->bmpInfo.bmiHeader.biWidth, this->bmpInfo.bmiHeader.biHeight, 
		screen_dc, this->capture_rect.left, this->capture_rect.top, SRCCOPY);
	
	this->LoadImageFromDIB(read_image);
}

bool WinScreenCapture::HasInvalidPremultipliedColors(const Color* image, const size_t num_pixels)
{
	const Color* pSrc = image;
	const Color* const pSrcEnd = pSrc + num_pixels;

	while (pSrc != pSrcEnd)
	{
		if (pSrc->r > pSrc->a || pSrc->g > pSrc->a || pSrc->b > pSrc->a)
		{
			return true;
		}

		++pSrc;
	}

	return false;
}

bool WinScreenCapture::LoadImageFromDIB(s3d::Image& read_image)
{
	
	{
		const void* memory = &(this->hBitmap);
		const BITMAPINFO* header = &(this->bmpInfo);
		const int32 depth = header->bmiHeader.biBitCount;
		size_t colorTableSize = 0;

		if (depth == 8)
		{
			colorTableSize = header->bmiHeader.biClrUsed ? header->bmiHeader.biClrUsed
				: (1 << header->bmiHeader.biBitCount);
		}
		else if (depth == 32 && header->bmiHeader.biCompression == BI_BITFIELDS)
		{
			colorTableSize = 3;
		}

		const size_t offsetSize = header->bmiHeader.biSize + colorTableSize * sizeof(RGBQUAD);
		const void* bitmapData = static_cast<const Byte*>(memory) + offsetSize;

		const bool reversed = header->bmiHeader.biHeight > 0;
		
		const int32 width = header->bmiHeader.biWidth;
		const int32 height = reversed ? header->bmiHeader.biHeight : -header->bmiHeader.biHeight;
		const bool hasAlpha = (depth == 32)
			&& !this->HasInvalidPremultipliedColors(static_cast<const Color*>(bitmapData), width* height);
		read_image.resize(width, height);

		ReaderView reader(this->lpPixel, this->bmpInfo.bmiHeader.biSizeImage);
		
		switch (depth)
		{
		case 8:
		{
			uint8 palette[1024];
			reader.read(palette);

			const uint32 rowSize = width + (width % 4 ? 4 - width % 4 : 0);
			const int32 lineStep = reversed ? -width : width;
			Color* pDstLine = read_image[reversed ? height - 1 : 0];

			if (uint8* const buffer = static_cast<uint8*>(std::malloc(rowSize)))
			{
				for (int32 y = 0; y < height; ++y)
				{
					if (y == height - 1)
					{
						reader.read(buffer, width);
					}
					else
					{
						reader.read(buffer, rowSize);
					}

					const uint8* pSrc = buffer;
					const Color* const pDstEnd = pDstLine + width;

					for (Color* pDst = pDstLine; pDst != pDstEnd; ++pDst)
					{
						const uint8* src = palette + (static_cast<size_t>(*pSrc++) << 2);

						pDst->set(src[2], src[1], src[0]);
					}

					pDstLine += lineStep;
				}

				std::free(buffer);
			}

			break;
		}
		case 24:
		case 32:
		{
			const size_t rowSize = (depth == 24) ? (width * 3 + width % 4) : (width * 4);
			const int32 depthBytes = depth / 8;
			const int32 lineStep = reversed ? -width : width;
			Color* pDstLine = read_image[reversed ? height - 1 : 0];

			if (uint8* const buffer = static_cast<uint8*>(std::malloc(rowSize)))
			{
				for (int32 y = 0; y < height; ++y)
				{
					if (y == height - 1)
					{
						reader.read(buffer, depthBytes * width);
					}
					else
					{
						reader.read(buffer, rowSize);
					}
				const Color* const pDstEnd = pDstLine + width;
					const uint8* pSrc = buffer;

					for (Color* pDst = pDstLine; pDst != pDstEnd; ++pDst)
					{
						pDst->set(pSrc[2], pSrc[1], pSrc[0], hasAlpha ? pSrc[3] : 255);

						pSrc += depthBytes;
					}

					pDstLine += lineStep;
				}

				std::free(buffer);
			}

			break;
		}
		}

	}
	
	return true;
}

//bool WinScreenCapture::LoadImageFromClipboard()
//{
//	// �Q�l�p.
//	const HANDLE hDIB = ::GetClipboardData(CF_DIB);
//
//	if (!hDIB)
//	{
//		return false;
//	}
//	const size_t memorySize = ::GlobalSize(hDIB);
//	if (const void* memory = reinterpret_cast<uint8*>(::GlobalLock(hDIB)))
//	{
//		const BITMAPINFO* header = static_cast<const BITMAPINFO*>(memory);
//		
//		const int32 depth = header->bmiHeader.biBitCount;
//
//		size_t colorTableSize = 0;
//
//		if (depth == 8)
//		{
//			colorTableSize = header->bmiHeader.biClrUsed ? header->bmiHeader.biClrUsed
//				: (1 << header->bmiHeader.biBitCount);
//		}
//		else if (depth == 32 && header->bmiHeader.biCompression == BI_BITFIELDS)
//		{
//			colorTableSize = 3;
//		}
//
//		const size_t offsetSize = header->bmiHeader.biSize + colorTableSize * sizeof(RGBQUAD);
//		const void* bitmapData = static_cast<const Byte*>(memory) + offsetSize;
//
//		const bool reversed = header->bmiHeader.biHeight > 0;
//
//		const int32 width = header->bmiHeader.biWidth;
//		const int32 height = reversed ? header->bmiHeader.biHeight : -header->bmiHeader.biHeight;
//		const bool hasAlpha = (depth == 32)
//			&& !this->HasInvalidPremultipliedColors(static_cast<const Color*>(bitmapData), width* height);
//		//image.resize(width, height);
//		read_image.resize(width, height);
//
//		ReaderView reader(bitmapData, memorySize - offsetSize);
//
//		switch (depth)
//		{
//		case 8:
//		{
//			uint8 palette[1024];
//			reader.read(palette);
//
//			const uint32 rowSize = width + (width % 4 ? 4 - width % 4 : 0);
//			const int32 lineStep = reversed ? -width : width;
//			Color* pDstLine = read_image[reversed ? height - 1 : 0];
//
//			if (uint8* const buffer = static_cast<uint8*>(std::malloc(rowSize)))
//			{
//				for (int32 y = 0; y < height; ++y)
//				{
//					if (y == height - 1)
//					{
//						reader.read(buffer, width);
//					}
//					else
//					{
//						reader.read(buffer, rowSize);
//					}
//
//					const uint8* pSrc = buffer;
//					const Color* const pDstEnd = pDstLine + width;
//
//					for (Color* pDst = pDstLine; pDst != pDstEnd; ++pDst)
//					{
//						const uint8* src = palette + (static_cast<size_t>(*pSrc++) << 2);
//
//						pDst->set(src[2], src[1], src[0]);
//					}
//
//					pDstLine += lineStep;
//				}
//
//				std::free(buffer);
//			}
//
//			break;
//		}
//		case 24:
//		case 32:
//		{
//			const size_t rowSize = (depth == 24) ? (width * 3 + width % 4) : (width * 4);
//			const int32 depthBytes = depth / 8;
//			const int32 lineStep = reversed ? -width : width;
//			Color* pDstLine = read_image[reversed ? height - 1 : 0];
//
//			if (uint8* const buffer = static_cast<uint8*>(std::malloc(rowSize)))
//			{
//				for (int32 y = 0; y < height; ++y)
//				{
//					if (y == height - 1)
//					{
//						reader.read(buffer, depthBytes * width);
//					}
//					else
//					{
//						reader.read(buffer, rowSize);
//					}
//					const Color* const pDstEnd = pDstLine + width;
//					const uint8* pSrc = buffer;
//
//					for (Color* pDst = pDstLine; pDst != pDstEnd; ++pDst)
//					{
//						pDst->set(pSrc[2], pSrc[1], pSrc[0], hasAlpha ? pSrc[3] : 255);
//
//						pSrc += depthBytes;
//					}
//
//					pDstLine += lineStep;
//				}
//
//				std::free(buffer);
//			}
//
//			break;
//		}
//		}
//
//	}
//	::GlobalUnlock(hDIB);
//
//	return true;
//}

