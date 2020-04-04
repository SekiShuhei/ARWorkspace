#pragma once

#include <Siv3D.hpp>

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#include <Windows.h>

class WinScreenCapture
{
public:
	WinScreenCapture();
	~WinScreenCapture();

	bool CaptureScreen(s3d::Image&, int x, int y, int width, int height);
	bool HasInvalidPremultipliedColors(const Color* image, const size_t num_pixels);

private:

	bool LoadImageFromDIB(s3d::Image&);

	DWORD GetBitmapImageSize(const BITMAPINFO& bitmap_info) const;

	
	RECT			capture_rect = RECT();
	
	HDC				hdc = HDC(); // Ç±ÇÍÇÕÇΩÇ‘ÇÒàÍéûïœêîÇ≈ÇÊÇ¢.
	HDC				hMemDC = HDC();
	BITMAPINFO		bmpInfo = BITMAPINFO();
	LPDWORD			lpPixel = LPDWORD();
	HBITMAP			hBitmap = HBITMAP();



};