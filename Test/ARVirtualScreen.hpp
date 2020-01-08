#pragma once

#include <Siv3D.hpp>


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

	// �X�V�����ƕ`��𕪂���@�}���`�X���b�h�ɂ��邽�Ԃ�.
	void Update();

	// �`��.
	void Draw();

	// �J�X�^���J�[�\��.
	// OS�J�[�\���Ɠ�������悤�ɁA���W�n��K�؂ɏ���.

	// ScreenCapture�֐����Ǌ��H





private:

	CustomCursor		custom_cursor;
	WinScreenCapture	screen_capture;
	SensorApiManager	sensor_manager;


	double	scale = 3.0;
	s3d::Point	capture_point = s3d::Point(0, 0);
	// ���ۂɃX�N���[������擾����ׂ��̈�.
	// ����͉�]�n���܂܂Ȃ�.
	s3d::Rect	capture_rect;
	



	s3d::DynamicTexture	texture;

	double	radian = 0.0;
	s3d::Image	capture_image;

};

