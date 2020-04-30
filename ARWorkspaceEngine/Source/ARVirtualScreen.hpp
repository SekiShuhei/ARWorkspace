#pragma once

#include "ARWorkspaceEngine.hpp"

#include <mutex>
#include <thread>

#include "SimpleCounter.hpp"
#include "ScreenRegion.hpp"
#include "ScreenRegionGuideView.hpp"
#include "WinScreenCapture.hpp"
#include "CustomCursor.hpp"

namespace ARWorkspace {

class ARVirtualScreen
{
public:

	//ARVirtualScreen() = delete;
	ARVirtualScreen(bool use_cauture_region_guide = true);

	ARVirtualScreen(const ARVirtualScreen&) = delete;
	ARVirtualScreen(const ARVirtualScreen&&) = delete;
	void operator =(const ARVirtualScreen&) = delete;

	~ARVirtualScreen();

	void Initialize();

	bool ReadConfigFile();
	bool WriteConfigFile();

private:

	bool GetCaptureRect();
	void Capture();


public:
	void Draw();

	
	const ScreenRegion& GetCaptureRegion() const
	{
		return this->capture_region;
	}
	inline void CaptureRegionUpdate()
	{
		this->capture_region_updated = true;
	}
	inline void CaptureSizeUpdate()
	{
		this->capture_size_updated = true;
	}
	void SetCaptureRegion(int arg_x, int arg_y, int arg_width, int arg_height);
	void SetCaptureRegionPosition(int arg_x, int arg_y);
	void SetCaptureRegionSize(int arg_width, int arg_height);
	void SetAutoResizeMode(bool arg_bool)
	{
		this->texture_auto_resize = arg_bool;
	}

private:

	void drawTexture();

	inline const s3d::Image& GetDrawImage() const
	{
		return this->capture_image[this->imageindex_drawing];
	};

private:

	CustomCursor		custom_cursor;
	WinScreenCapture	screen_capture;
	
	std::thread			capture_thread;
	bool				capture_thread_run = false;
	const bool			capture_region_guide_enable = true;
	std::thread			capture_region_guide_thread;
	bool				capture_region_guide_thread_run = false;
	bool				capture_region_guide_drawing = false;



	s3d::Point	capture_point = s3d::Point(0, 0);
	// ÀÛ‚ÉƒXƒNƒŠ[ƒ“‚©‚çæ“¾‚·‚é‚×‚«—Ìˆæ.
	// Œ»ó‚Í‰ñ“]Œn‚ğŠÜ‚Ü‚È‚¢.
	ScreenRegion		capture_region;
	ScreenRegionGuideView	capture_region_guide = ScreenRegionGuideView(capture_region, 10);
	
	// •`‰æŒn.
	std::mutex			mutex;
	int					imageindex_reading = 0;
	int					imageindex_standby = 1;
	int					imageindex_drawing = 2;
	
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
	std::unique_ptr<s3d::DynamicTexture>	p_texture;
	
	s3d::Image	capture_image[3] = 
	{
		s3d::Image(),
		s3d::Image(),
		s3d::Image()
	};

	bool capture_region_updated = false;
	bool capture_size_updated = false;
	SimpleCounter	capture_region_guide_counter = SimpleCounter(10);
	SimpleCounter	texture_reflesh_counter = SimpleCounter(3);
	
public:
	double	scale = 3.0;
	double	radian = 0.0;
	bool texture_auto_resize = false;
};

}