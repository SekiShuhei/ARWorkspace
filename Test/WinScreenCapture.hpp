#pragma once

#include <Siv3D.hpp>

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
	HWND			desktop = HWND();
	
	HDC				hdc = HDC(); // Ç±ÇÍÇÕÇΩÇ‘ÇÒàÍéûïœêîÇ≈ÇÊÇ¢.
	HDC				hMemDC = HDC();
	BITMAPINFO		bmpInfo = BITMAPINFO();
	LPDWORD			lpPixel = LPDWORD();
	HBITMAP			hBitmap = HBITMAP();



};