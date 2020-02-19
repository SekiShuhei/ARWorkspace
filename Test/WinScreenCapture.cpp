
#include <math.h>

#include "WinScreenCapture.hpp"



WinScreenCapture::WinScreenCapture() :
	bmpInfo(), desktop()
{
	this->capture_rect = RECT{100,100,400,400}; //kari
}


WinScreenCapture::~WinScreenCapture()
{
	//自らlpPixelを解放するべからず
	DeleteDC(this->hMemDC);
	DeleteObject(this->hBitmap);  //BMPを削除した時、lpPixelも自動的に解放される

}

DWORD WinScreenCapture::GetBitmapImageSize(const BITMAPINFO& bitmap_info) const
{
	// 画像データサイズを正しく計算して入れる.
	// www5d.biglobe.ne.jp/~noocyte/Programming/Windows/BmpFileFormat.html#CalcBitmapSize


	DWORD lineSizeDW = 0;
	//BPP＝1, (2, ) 4, 8 の場合
	switch (bitmap_info.bmiHeader.biBitCount)
	{
	case 1:
	case 2:
	case 4:
		//case 8: //8はどちらでもよいらしい
	{
		DWORD pixelsPerDW = 8 * sizeof(DWORD) / bitmap_info.bmiHeader.biBitCount; // DWORD 内の画素数

		// 走査線のサイズ (DWORD 数) を計算する．
		// これは width / pixelsPerDW の小数部を切り上げた値である．
		lineSizeDW = ceil(bitmap_info.bmiHeader.biWidth / pixelsPerDW);
		break;
	}
	//BPP＝8, 16，24，32 の場合
	case 8:
	case 16:
	case 24:
	case 32:
	{
		DWORD bytesPerPixel = bitmap_info.bmiHeader.biBitCount / 8; // １画素当たりのバイト数

		// まず，走査線の正味のバイト数を計算する．
		lineSizeDW = bytesPerPixel * bitmap_info.bmiHeader.biWidth;

		// lineSizeDW を実際のサイズ (DWORD 数) にするため，
		// sizeof(DWORD) で割る (小数部切り上げ)．
		lineSizeDW = ceil(lineSizeDW / sizeof(DWORD));
		break;
	}
	default:
		break;
	}
	//次に，走査線のサイズ(バイト数) lineSize とビットマップデータの全バイト数 imageSize を求める．
	DWORD lineSize = lineSizeDW * sizeof(DWORD);
	return lineSize * bitmap_info.bmiHeader.biHeight;

}


bool WinScreenCapture::CaptureScreen(s3d::Image& read_image, int x, int y, int width, int height)
{
	

	// キャプチャ領域.
	this->capture_rect.left = x;
	this->capture_rect.top = y;
	this->capture_rect.right = x + width;
	this->capture_rect.bottom = y + height;

	//スクリーンの情報を得る
	this->desktop = GetDesktopWindow();

	//DIBの情報を設定する
	this->bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->bmpInfo.bmiHeader.biWidth = width;
	this->bmpInfo.bmiHeader.biHeight = height;
	this->bmpInfo.bmiHeader.biPlanes = 1;
	this->bmpInfo.bmiHeader.biBitCount = 32;
	this->bmpInfo.bmiHeader.biCompression = BI_RGB;
	this->bmpInfo.bmiHeader.biSizeImage = this->GetBitmapImageSize(this->bmpInfo);

	//DIBSection作成
	this->hdc = GetDC(NULL);
	
	HDC hPrevMemDC = this->hMemDC;
	HBITMAP hPrevBitmap = this->hBitmap;
	this->hBitmap = ::CreateDIBSection(this->hdc, &this->bmpInfo, DIB_RGB_COLORS, (void**)&this->lpPixel, NULL, 0);
	bool error = true;
	if (this->hBitmap != NULL)
	{	
		this->hMemDC = ::CreateCompatibleDC(this->hdc);
		if (this->hMemDC != NULL)
		{
			::SelectObject(this->hMemDC, this->hBitmap);
			error = false;
		}
	} else {
		int a =0;
	}
	if (hPrevBitmap != NULL)
	{
		::DeleteObject(hPrevBitmap);  //BMPを削除した時、lpPixelも自動的に解放される
	}
	if (hPrevMemDC != NULL)
	{
		::DeleteDC(hPrevMemDC);
	}
	::ReleaseDC(NULL, this->hdc);

	if (error)
	{
		return false;
	}

	// ここの手順は見直す余地がかなりあるっぽい.

	bool result = false;

	//スクリーンをDIBSectionにコピー
	this->hdc = ::GetDC(this->desktop);
	::BitBlt(this->hMemDC, 0, 0, this->bmpInfo.bmiHeader.biWidth, this->bmpInfo.bmiHeader.biHeight, 
		this->hdc, this->capture_rect.left, this->capture_rect.top, SRCCOPY);
	if (this->hdc != NULL)
	{
		::ReleaseDC(this->desktop, this->hdc);
	}

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
		// ビットマップデータの先頭へのアドレス.
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

		//ReaderView reader(bitmapData, this->bmpInfo.bmiHeader.biSizeImage);
		ReaderView reader(this->lpPixel, this->bmpInfo.bmiHeader.biSizeImage);
		//ReaderView reader(bitmapData, memorySize - offsetSize);

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
//	// 参考用.
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

