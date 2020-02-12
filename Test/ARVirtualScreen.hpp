#pragma once

#include <Siv3D.hpp>

#include <mutex>
#include <thread>

#include "DisplayRegion.hpp"
#include "WinScreenCapture.hpp"
#include "CustomCursor.hpp"
#include "SensorApiManager.hpp"





class ARVirtualScreen
{
public:

	ARVirtualScreen();

	// �Ă��Ƃ��E�E�E.
	ARVirtualScreen(const ARVirtualScreen&) = delete;
	ARVirtualScreen(const ARVirtualScreen&&) = delete;
	void operator =(const ARVirtualScreen&) = delete;

	~ARVirtualScreen();

	void initialize();

	// ���[�U�[�ݒ�̃��[�h.
	bool LoadUserSetting();

	// �Z���T�n.
	// COM�n�K�{���ۂ�.

	// �Z���T���̉���.
	// �f�[�^����.
	// �W�F�X�`���[����.

	// ���[�h�͕����z�肷��.
	// EXE���x���ł킯��H.




	// ���˗̈�̌v�Z.
	// �`���̃E�B���h�E�T�C�Y��n��.
	bool GetCaptureRect();

	// �X�N���[���L���v�`��.

	void Capture();

	// �`��.
	void Draw();

	// �J�X�^���J�[�\��.
	// OS�J�[�\���Ɠ�������悤�ɁA���W�n��K�؂ɏ���.

	// ScreenCapture�֐����Ǌ��H





private:

	CustomCursor		custom_cursor;
	WinScreenCapture	screen_capture;
	SensorApiManager	sensor_manager;

	// �L���v�`���n.
	std::thread			capture_thread;
	bool				capture_thread_run = false;

	double	scale = 3.0;
	s3d::Point	capture_point = s3d::Point(0, 0);
	// ���ۂɃX�N���[������擾����ׂ��̈�.
	// ����͉�]�n���܂܂Ȃ�.
	DisplayRegion	capture_region;

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
	
	double	radian = 0.0;
	s3d::Image	capture_image[3] = 
	{
		s3d::Image(),
		s3d::Image(),
		s3d::Image()
	};

	bool capture_region_updated = false;
	int texture_reflesh_count = 0;
	int texture_reflesh_count_max = 3;
	
};

