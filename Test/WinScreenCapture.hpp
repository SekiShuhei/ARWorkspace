#pragma once

#include <Siv3D.hpp>

#include <Windows.h>


// WinAPIで任意のスクリーン領域をキャプチャしクリップボードに保存する.
class WinScreenCapture
{
public:
	WinScreenCapture();
	~WinScreenCapture();

	bool CaptureScreen(int x, int y, int width, int height);

	bool HasInvalidPremultipliedColors(const Color* image, const size_t num_pixels);

	bool LoadImageFromDIB();
	bool LoadImageFromClipboard();

	s3d::Image& GetImage()
	{
		return this->captured_image;
	}
private:

	DWORD GetBitmapImageSize(const BITMAPINFO& bitmap_info) const;

	
	RECT			capture_rect = RECT();
	HWND			desktop = HWND();
	
	HDC				hdc = HDC(); // これはたぶん一時変数でよい.
	HDC				hMemDC = HDC();
	BITMAPINFO		bmpInfo = BITMAPINFO();
	LPDWORD			lpPixel = LPDWORD();
	HBITMAP			hBitmap = HBITMAP();

	// DIBからクリップボードを介さずに直接S3DImageに落とし込む実験.
	s3d::Image		captured_image = s3d::Image();


};