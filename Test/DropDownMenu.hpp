#pragma once

#include <Siv3D.hpp>

namespace ARWorkspace {
namespace CustomGUI {

using DropDownMenuDrawEvent = std::function<int(int)>;
class DropDownMenu
{
public:
	//DropDownMenu() = 0;
	DropDownMenu(DropDownMenuDrawEvent arg_draw_event);
public:
	int Draw(s3d::Vec2 arg_position, int arg_width);

	void SetLabel(s3d::String arg_label)
	{
		this->top_label = arg_label;
	}
	void SetPosition(s3d::Vec2 arg_position)
	{
		this->position = arg_position;
	}
	void SetWidth(int arg_width)
	{
		this->width = arg_width;
	}

private:
	bool					is_open = false;

	s3d::String				top_label = U"";
	s3d::Vec2				position = Vec2(0,0);
	int						width = 100;
	DropDownMenuDrawEvent	draw_event = [](int){return 0;};
};

}
}

