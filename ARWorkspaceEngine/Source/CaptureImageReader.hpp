#pragma once

#include <mutex>
#include <optional>
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
		// ‘½­–³‘Ê‚¾‚ª‚Æ‚è‚ ‚¦‚¸.
		this->capture_region = arg_capture_region;
	}
	inline bool IsDrawingStandby() const
	{
		return (this->imageindex_standby >= 0);
	}

	inline const s3d::Image& GetDrawImage()
	{
		std::lock_guard<std::mutex>	lock(this->mutex);
		if (this->imageindex_standby >= 0)
		{
			this->imageindex_drawing = this->imageindex_standby;
			this->imageindex_standby = -1;
		}
		if (this->imageindex_drawing >= 0)
		{
			return this->capture_image[this->imageindex_drawing];
		}
		assert("CaptureImageReader Not Drawing Standby");
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

	// CaptureImageƒNƒ‰ƒX‚Æ‚µ‚Ä“Æ—§‚³‚¹‚é—\’è.
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
