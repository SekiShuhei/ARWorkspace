#pragma once

#include <Siv3D.hpp>

#include <mutex>
#include <thread>

#include "SimpleCounter.hpp"
#include "DisplayRegion.hpp"
#include "DisplayRegionGuideView.hpp"
#include "WinScreenCapture.hpp"
#include "CustomCursor.hpp"
#include "SensorApiManager.hpp"



namespace ARWorkspace {

class ARVirtualScreen
{
public:

	ARVirtualScreen();

	// てきとう・・・.
	ARVirtualScreen(const ARVirtualScreen&) = delete;
	ARVirtualScreen(const ARVirtualScreen&&) = delete;
	void operator =(const ARVirtualScreen&) = delete;

	~ARVirtualScreen();

	void Initialize();

	bool ReadConfigFile();
	bool WriteConfigFile();

	// センサ系.
	// COM系必須っぽい.

	// センサ情報の解釈.
	// データ整流.
	// ジェスチャー解釈.

	// モードは複数想定する.
	// EXEレベルでわける？.



private:

	bool GetCaptureRect();
	void Capture();


public:
	// 描画.
	void Draw();

	// カスタムカーソル.
	// OSカーソルと同期するように、座標系を適切に処理.
	// ↑いらない？.

	const DisplayRegion& GetCaptureRegion() const
	{
		return this->capture_region;
	}
	void CaptureRegionUpdate()
	{
		this->capture_region_updated = true;
	}
	void CaptureSizeUpdate()
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
	SensorApiManager	sensor_manager;

	// キャプチャ系.
	std::thread			capture_thread;
	bool				capture_thread_run = false;
	std::thread			capture_region_guide_thread;
	bool				capture_region_guide_thread_run = false;
	bool				capture_region_guide_drawing = false;

	s3d::Point	capture_point = s3d::Point(0, 0);
	// 実際にスクリーンから取得するべき領域.
	// 現状は回転系を含まない.
	DisplayRegion	capture_region;
	DisplayRegionGuideView	capture_region_guide = DisplayRegionGuideView(capture_region, 20);
	
	// 描画系.
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
	
	// テクスチャ拡縮関連.
	// アクセサ経由に変更する
public:
	double	scale = 3.0;
	double	radian = 0.0;
	bool texture_auto_resize = false;
};

}