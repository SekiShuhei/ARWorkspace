#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B

#include <math.h>

//#include <Windows.h>
//#include <atlbase.h>

#include "WinScreenCapture.hpp"



WinScreenCapture::WinScreenCapture() :
	bmpInfo(), desktop()
{
	this->capture_rect = RECT{100,100,400,400}; //kari
}


WinScreenCapture::~WinScreenCapture()
{
	//����lpPixel���������ׂ��炸
	DeleteDC(this->hMemDC);
	DeleteObject(this->hBitmap);  //BMP���폜�������AlpPixel�������I�ɉ�������

}

bool WinScreenCapture::CaptureScreen(int x, int y, int width, int height)
{
	

	// �L���v�`���̈�.
	this->capture_rect.left = x;
	this->capture_rect.top = y;
	this->capture_rect.right = x + width;
	this->capture_rect.bottom = y + height;

	//�X�N���[���̏��𓾂�
	this->desktop = GetDesktopWindow();

	//DIB�̏���ݒ肷��
	this->bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->bmpInfo.bmiHeader.biWidth = this->capture_rect.right - this->capture_rect.left;
	this->bmpInfo.bmiHeader.biHeight = this->capture_rect.bottom - this->capture_rect.top;
	this->bmpInfo.bmiHeader.biPlanes = 1;
	this->bmpInfo.bmiHeader.biBitCount = 32;
	this->bmpInfo.bmiHeader.biCompression = BI_RGB;

	{
	// �摜�f�[�^�T�C�Y�𐳂����v�Z���ē����.
	// www5d.biglobe.ne.jp/~noocyte/Programming/Windows/BmpFileFormat.html#CalcBitmapSize

	
		DWORD lineSizeDW = 0;
		//BPP��1, (2, ) 4, 8 �̏ꍇ
		switch (this->bmpInfo.bmiHeader.biBitCount)
		{
		case 1:
		case 2:
		case 4:
		//case 8: //8�͂ǂ���ł��悢�炵��
		{
			DWORD pixelsPerDW = 8 * sizeof(DWORD) / this->bmpInfo.bmiHeader.biBitCount; // DWORD ���̉�f��

			// �������̃T�C�Y (DWORD ��) ���v�Z����D
			// ����� width / pixelsPerDW �̏�������؂�グ���l�ł���D
			lineSizeDW = ceil(width / pixelsPerDW);
			break;
		}
		//BPP��8, 16�C24�C32 �̏ꍇ
		case 8:
		case 16:
		case 24:
		case 32:
		{
			DWORD bytesPerPixel = this->bmpInfo.bmiHeader.biBitCount / 8; // �P��f������̃o�C�g��

			// �܂��C�������̐����̃o�C�g�����v�Z����D
			lineSizeDW = bytesPerPixel * width;

			// lineSizeDW �����ۂ̃T�C�Y (DWORD ��) �ɂ��邽�߁C
			// sizeof(DWORD) �Ŋ��� (�������؂�グ)�D
			lineSizeDW = ceil(lineSizeDW / sizeof(DWORD));
			break;
		}
		default:
			break;
		}
		//���ɁC�������̃T�C�Y(�o�C�g��) lineSize �ƃr�b�g�}�b�v�f�[�^�̑S�o�C�g�� imageSize �����߂�D
		DWORD lineSize = lineSizeDW * sizeof(DWORD);
		DWORD imageSize = lineSize * height;

		this->bmpInfo.bmiHeader.biSizeImage = imageSize;
	}
	//DIBSection�쐬
	this->hdc = GetDC(NULL);
	// �O��Bitmap�������Ȃ��ƁH�H.
	if (this->hBitmap != NULL)
	{
		if (this->hMemDC != NULL)
		{
			DeleteDC(this->hMemDC);
		}
		DeleteObject(this->hBitmap);  //BMP���폜�������AlpPixel�������I�ɉ�������
	}
	this->hBitmap = CreateDIBSection(this->hdc, &this->bmpInfo, DIB_RGB_COLORS, (void**)&this->lpPixel, NULL, 0);
	bool error = true;
	if (this->hBitmap != NULL)
	{
		
		this->hMemDC = CreateCompatibleDC(this->hdc);
		if (this->hMemDC != NULL)
		{
			SelectObject(this->hMemDC, this->hBitmap);
			error = false;
		}
	}
	ReleaseDC(NULL, this->hdc);

	if (error)
	{
		return false;
	}

	// �����̎菇�͌������]�n�����Ȃ肠����ۂ�.

	bool result = false;

	//�X�N���[����DIBSection�ɃR�s�[
	this->hdc = GetDC(this->desktop);
	BitBlt(this->hMemDC, 0, 0, this->bmpInfo.bmiHeader.biWidth, this->bmpInfo.bmiHeader.biHeight, 
		this->hdc, this->capture_rect.left, this->capture_rect.top, SRCCOPY);
	if (this->hdc != NULL)
	{
		ReleaseDC(this->desktop, this->hdc);
	}

	///DIB���N���b�v�{�[�h�ɃR�s�[
	//if (OpenClipboard(NULL) == NULL)
	//{
	//	return false;
	//}
	//if (EmptyClipboard() == NULL)
	//{
	//	return false;
	//}
	//result = (SetClipboardData(CF_BITMAP, hBitmap) != NULL);
	//CloseClipboard();
	
	////////
	// [����]�X�N���[����������DC���N���b�v�{�[�h��Siv3D::Image.
	// ��
	// ������Image����郁�\�b�h��p�ӂ��čœK������.
	////////
	this->LoadImageFromDIB();

	//{
	//	HWND hWnd = FindWindow(LPCWSTR("SystemTray_Main"), NULL);
	//
	//	HWND m_hWnd = static_cast<HWND>(Siv3DEngine::Get<ISiv3DWindow>()->getHandle());
	//	if (!::OpenClipboard(m_hWnd))
	//	{
	//		return false;
	//	}
	//
	//	this->LoadImageFromClipboard();
	//	CloseClipboard();
	//
	//}
	//
	//return result;
}

// ���̂܂܈��p.
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

bool WinScreenCapture::LoadImageFromDIB()
{
	// DIB�n���h���͂��̂܂܎g��.
	//const HANDLE hDIB = ::GetClipboardData(CF_DIB);

	//if (!hDIB)
	//{
	//	return;
	//}
	// �f�[�^�T�C�Y�̎擾���ł����GlobalSize�łȂ��Ă悢�͂�.
	//�o�C�g�P�ʂ̃f�[�^�T�C�Y.

	// �摜�f�[�^�T�C�Y�̌v�Z���@.
	//www5d.biglobe.ne.jp / ~noocyte / Programming / Windows / BmpFileFormat.html#CalcBitmapSize


	//const size_t memorySize = (this->hBitmap);
	//const size_t memorySize = ::GlobalSize(hDIB);
	// ����.
	// GlobalLock�̓N���b�v�{�[�h�����̂��߁H�Ƃ���Εs�v����.
	// GlobalLock�̓������u���b�N�ւ̐擪�ւ̃|�C���^��Ԃ�.
	//if (const void* memory = reinterpret_cast<uint8*>(::GlobalLock(hDIB)))
	{
		// �r�b�g�}�b�v�f�[�^�̐擪�ւ̃A�h���X.
		const void* memory = &(this->hBitmap);

		// ���łɎ擾�ς݂̂͂�.
		//const BITMAPINFO* header = static_cast<const BITMAPINFO*>(memory);
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
		//image.resize(width, height);
		this->captured_image.resize(width, height);

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
			Color* pDstLine = this->captured_image[reversed ? height - 1 : 0];

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
			Color* pDstLine = this->captured_image[reversed ? height - 1 : 0];

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
	// �v�m�F.
	//::GlobalUnlock(hDIB);

	return true;
}

bool WinScreenCapture::LoadImageFromClipboard()
{
	const HANDLE hDIB = ::GetClipboardData(CF_DIB);

	if (!hDIB)
	{
		return false;
	}
	const size_t memorySize = ::GlobalSize(hDIB);
	if (const void* memory = reinterpret_cast<uint8*>(::GlobalLock(hDIB)))
	{
		const BITMAPINFO* header = static_cast<const BITMAPINFO*>(memory);
		
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
		//image.resize(width, height);
		this->captured_image.resize(width, height);

		ReaderView reader(bitmapData, memorySize - offsetSize);

		switch (depth)
		{
		case 8:
		{
			uint8 palette[1024];
			reader.read(palette);

			const uint32 rowSize = width + (width % 4 ? 4 - width % 4 : 0);
			const int32 lineStep = reversed ? -width : width;
			Color* pDstLine = this->captured_image[reversed ? height - 1 : 0];

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
			Color* pDstLine = this->captured_image[reversed ? height - 1 : 0];

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
	::GlobalUnlock(hDIB);

	return true;
}
