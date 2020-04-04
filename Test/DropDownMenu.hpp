#pragma once

#include <Siv3D.hpp>

namespace ARWorkspace {
namespace CustomGUI {

using DropDownMenuDrawEvent = std::function<bool()>;
class DropDownMenu
{
public:
	//DropDownMenu() = 0;
	DropDownMenu(DropDownMenuDrawEvent arg_draw_event);
public:
	bool Draw(s3d::Vec2 arg_position);

	void SetLabel(s3d::String arg_label)
	{
		this->top_label = arg_label;
	}
	void SetPosition(s3d::Vec2 arg_position)
	{
		this->position = arg_position;
	}

private:
	bool					is_open = false;

	s3d::String				top_label = U"";
	s3d::Vec2				position = Vec2(0,0);
	DropDownMenuDrawEvent	draw_event = [](){return false;};
};

}
}

