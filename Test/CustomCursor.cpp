#include "CustomCursor.hpp"


#include <siv3d.hpp>

CustomCursor::CustomCursor()
{

}


CustomCursor::~CustomCursor()
{

}

void CustomCursor::DrawAt(int arg_pos_x, int arg_pos_y)
{
	// この関数の実装がたぶん参考になる.
	//s3d::ScreenCapture::GetFrame();

	// Siv3D.ウィンドウの位置を取得して画面のカーソルと位置合わせする

	this->pos_x = arg_pos_x;
	this->pos_y = arg_pos_y;

	{

		s3d::Line(this->pos_x - this->size, this->pos_y,
				  this->pos_x + this->size, this->pos_y).draw(5, Palette::Black);
		s3d::Line(this->pos_x, this->pos_y - this->size,
				  this->pos_x, this->pos_y + this->size).draw(5, Palette::Black);


	}

}
