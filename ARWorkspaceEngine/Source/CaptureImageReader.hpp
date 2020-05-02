#pragma once

#include <mutex>
#include <optional>
#include <functional>
#include <Siv3D.hpp>
#include "WinScreenCapture.hpp"
#include "ScreenRegion.hpp"
#include "CaptureImage.hpp"

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
		return (this->imageindex_drawing >= 0);
	}

	const s3d::Image& GetDrawImage();

	bool DrawImage(std::function<void(const CaptureImage&)> draw_func);

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
	
	CaptureImage	capture_image[3] = { CaptureImage(), CaptureImage(), CaptureImage() };

	//s3d::Image	capture_image[3] =
	//{
	//	s3d::Image(),
	//	s3d::Image(),
	//	s3d::Image()
	//};

};

}
