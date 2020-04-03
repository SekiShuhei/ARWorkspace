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

	// �Ă��Ƃ��E�E�E.
	ARVirtualScreen(const ARVirtualScreen&) = delete;
	ARVirtualScreen(const ARVirtualScreen&&) = delete;
	void operator =(const ARVirtualScreen&) = delete;

	~ARVirtualScreen();

	void Initialize();

	bool ReadConfigFile();
	bool WriteConfigFile();

	// �Z���T�n.
	// COM�n�K�{���ۂ�.

	// �Z���T���̉���.
	// �f�[�^����.
	// �W�F�X�`���[����.

	// ���[�h�͕����z�肷��.
	// EXE���x���ł킯��H.



private:

	bool GetCaptureRect();
	void Capture();


public:
	// �`��.
	void Draw();

	// �J�X�^���J�[�\��.
	// OS�J�[�\���Ɠ�������悤�ɁA���W�n��K�؂ɏ���.
	// ������Ȃ��H.

	DisplayRegion& GetCaptureRegion()
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

	// �L���v�`���n.
	std::thread			capture_thread;
	bool				capture_thread_run = false;
	std::thread			capture_region_guide_thread;
	bool				capture_region_guide_thread_run = false;
	bool				capture_region_guide_drawing = false;

	s3d::Point	capture_point = s3d::Point(0, 0);
	// ���ۂɃX�N���[������擾����ׂ��̈�.
	// ����͉�]�n���܂܂Ȃ�.
	DisplayRegion	capture_region;
	DisplayRegionGuideView	capture_region_guide = DisplayRegionGuideView(capture_region, 20);
	
	// �`��n.
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
	
	// �e�N�X�`���g�k�֘A.
	// GUI����Q�ƃA�N�Z�X���邽�߂Ƀp�u���b�N�ɂ��Ă��邪�A���[��.
public:
	double	scale = 3.0;
	double	radian = 0.0;
	bool texture_auto_resize = false;
};

}