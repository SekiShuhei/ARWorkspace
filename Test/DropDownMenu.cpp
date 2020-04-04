#include "DropDownMenu.hpp"

namespace ARWorkspace {
namespace CustomGUI {

DropDownMenu::DropDownMenu(DropDownMenuDrawEvent arg_draw_event) :
	draw_event(arg_draw_event)
{

}
bool DropDownMenu::Draw(s3d::Vec2 arg_position, int arg_width)
{
	this->position = arg_position;
	this->width = arg_width;
	if (! this->is_open)
	{
		if (s3d::SimpleGUI::Button(U"▼" + this->top_label, this->position, this->width))
		{
			this->is_open = true;
		}
		return true;
	}
	// 一番上にも閉じるボタン配置.
	if (s3d::SimpleGUI::Button(U"▲" + this->top_label, this->position, this->width))
	{
		this->is_open = false;
	}
	// 改行すみ高さ値を渡す必要がある.

	bool result = this->draw_event();

	// 閉じるボタン表示.

	return result;
}


}
}