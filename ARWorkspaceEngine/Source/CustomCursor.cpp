#include "CustomCursor.hpp"


#include <siv3d.hpp>

namespace ARWorkspace {

CustomCursor::CustomCursor()
{

}


CustomCursor::~CustomCursor()
{

}

void CustomCursor::DrawAt(int arg_pos_x, int arg_pos_y)
{
	// ���̊֐��̎��������Ԃ�Q�l�ɂȂ�.
	//s3d::ScreenCapture::GetFrame();

	// Siv3D.�E�B���h�E�̈ʒu���擾���ĉ�ʂ̃J�[�\���ƈʒu���킹����

	this->pos_x = arg_pos_x;
	this->pos_y = arg_pos_y;

	{

		s3d::Line(this->pos_x - this->size, this->pos_y,
				  this->pos_x + this->size, this->pos_y).draw(5, Palette::Black);
		s3d::Line(this->pos_x, this->pos_y - this->size,
				  this->pos_x, this->pos_y + this->size).draw(5, Palette::Black);


	}

}

}