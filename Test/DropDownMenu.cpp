#include "DropDownMenu.hpp"

namespace ARWorkspace {
namespace CustomGUI {

DropDownMenu::DropDownMenu(DropDownMenuDrawEvent arg_draw_event) :
	draw_event(arg_draw_event)
{

}
int DropDownMenu::Draw(s3d::Vec2 arg_position, int arg_width)
{
	this->position = arg_position;
	this->width = arg_width;
	int y = this->position.y;
	int h = 20;

	if (! this->is_open)
	{
		if (s3d::SimpleGUI::Button(U"¥" + this->top_label, 
			s3d::Vec2(this->position.x, y), this->width))
		{
			this->is_open = true;
		}
	} else {
		if (s3d::SimpleGUI::Button(U"£" + this->top_label, 
			s3d::Vec2(this->position.x, y), this->width))
		{
			this->is_open = false;
		}
		y += h;

		h = this->draw_event(y);

		if (s3d::SimpleGUI::Button(U"close", 
			s3d::Vec2(this->position.x, y), this->width))
		{
			this->is_open = false;
		}
	}
	// ‚‚³‚ğ•Ô‚·.
	y += h;
	return y;
}


}
}