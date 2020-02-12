#pragma once

#include <Siv3D.hpp>

#include <mutex>
#include <thread>

#include "SimpleCounter.hpp"
#include "DisplayRegion.hpp"
#include "WinScreenCapture.hpp"
#include "CustomCursor.hpp"
#include "SensorApiManager.hpp"





class ARVirtualScreen
{
public:

	ARVirtualScreen();

	// てきとう・・・.
	ARVirtualScreen(const ARVirtualScreen&) = delete;
	ARVirtualScreen(const ARVirtualScreen&&) = delete;
	void operator =(const ARVirtualScreen&) = delete;

	~ARVirtualScreen();

	void initialize();

	// ユーザー設定のロード.
	bool LoadUserSetting();

	// センサ系.
	// COM系必須っぽい.

	// センサ情報の解釈.
	// データ整流.
	// ジェスチャー解釈.

	// モードは複数想定する.
	// EXEレベルでわける？.




	// 投射領域の計算.
	// 描画先のウィンドウサイズを渡す.
	bool GetCaptureRect();

	// スクリーンキャプチャ.

	void Capture();

	// 描画.
	void Draw();

	// カスタムカーソル.
	// OSカーソルと同期するように、座標系を適切に処理.

	// ScreenCapture関数を管轄？

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

	double	scale = 3.0;
	s3d::Point	capture_point = s3d::Point(0, 0);
	// 実際にスクリーンから取得するべき領域.
	// 現状は回転系を含まない.
	DisplayRegion	capture_region;

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
	
	double	radian = 0.0;
	s3d::Image	capture_image[3] = 
	{
		s3d::Image(),
		s3d::Image(),
		s3d::Image()
	};

	bool capture_region_updated = false;
	SimpleCounter	texture_reflesh_counter = SimpleCounter(3);
};

