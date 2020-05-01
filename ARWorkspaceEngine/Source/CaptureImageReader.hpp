#pragma once

#include <mutex>
#include <Siv3D.hpp>
#include "WinScreenCapture.hpp"
#include "ScreenRegion.hpp"

namespace ARWorkspace {

class CaptureImageReader
{
public:
	CaptureImageReader();
	~CaptureImageReader();

public:

public:
	void Start();
	void Capture();

	inline void SetCaptureEnable(const bool arg_capture_enable)
	{
		this->capture_enable = arg_capture_enable;
	}

	inline void SetCaptureRegion(ScreenRegion arg_capture_region)
	{
		// �������ʂ����Ƃ肠����.
		this->capture_region = arg_capture_region;
	}

	inline const s3d::Image& GetDrawImage() const
	{
		return this->capture_image[this->imageindex_drawing];
	};


private:
	bool				capture_enable = true;

	WinScreenCapture	screen_capture;
	
	ScreenRegion		capture_region;

	std::thread			capture_thread;
	bool				capture_thread_run = false;
	std::mutex			mutex;

	int					imageindex_reading = 0;
	int					imageindex_standby = 1;
	int					imageindex_drawing = 2;

	// CaptureImage�N���X�Ƃ��ēƗ�������\��.
private:
	enum class ImageState
	{
		not_initialized = -1,
		reading = 0,
		standby,
		drawing,
		drawed
	};
	ImageState			image_state[3]
	{
		ImageState::not_initialized ,
		ImageState::not_initialized ,
		ImageState::not_initialized
	};
	
	s3d::Image	capture_image[3] =
	{
		s3d::Image(),
		s3d::Image(),
		s3d::Image()
	};

};

}
