#pragma once

#include "ARWorkspaceEngine.hpp"

#include <mutex>
#include <thread>

#include "SimpleCounter.hpp"
#include "ScreenRegion.hpp"
#include "ScreenRegionGuideView.hpp"
#include "CustomCursor.hpp"
#include "CaptureImageReader.hpp"

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
	void SetCapturePosition(int x, int y, double angle, double scale);

private:

	void drawTexture();

	
private:

	CustomCursor		custom_cursor;
	CaptureImageReader	capture_reader;

	const bool			capture_region_guide_enable = true;
	std::thread			capture_region_guide_thread;
	bool				capture_region_guide_thread_run = false;
	bool				capture_region_guide_drawing = false;



	s3d::Point	capture_point = s3d::Point(0, 0);
	// 実際にスクリーンから取得するべき領域.
	// 現状は回転系を含まない.
	ScreenRegion		capture_region;
	ScreenRegionGuideView	capture_region_guide = ScreenRegionGuideView(capture_region, 10);
	
	std::unique_ptr<s3d::DynamicTexture>	p_texture;
	
	bool capture_region_updated = false;
	bool capture_size_updated = false;
	SimpleCounter	capture_region_guide_counter = SimpleCounter(10);
	SimpleCounter	texture_reflesh_counter = SimpleCounter(3);
	
public:
	double	scale = 1.0;
	double	angle = 0.0;
	bool texture_auto_resize = false;

	int		texture_offset_margin = 300;

private:
	s3d::Font	font = Font(30); //デバッグ用.
};

}