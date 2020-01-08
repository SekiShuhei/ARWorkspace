#pragma once

#include <Siv3D.hpp>

#include <Windows.h>


// WinAPI�ŔC�ӂ̃X�N���[���̈���L���v�`�����N���b�v�{�[�h�ɕۑ�����.
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

	// WinAPI : �f�X�N�g�b�v�L���v�`����DIB���N���b�v�{�[�h
	//...

	RECT			capture_rect = RECT();
	HWND			desktop = HWND();
	
	HDC				hdc = HDC(); // ����͂��Ԃ�ꎞ�ϐ��ł悢.
	HDC				hMemDC = HDC();
	BITMAPINFO		bmpInfo = BITMAPINFO();
	LPDWORD			lpPixel = LPDWORD();
	HBITMAP			hBitmap = HBITMAP();

	// DIB����N���b�v�{�[�h������ɒ���S3DImage�ɗ��Ƃ����ގ���.
	s3d::Image		captured_image = s3d::Image();


};