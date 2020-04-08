#pragma once

#include <Siv3D.hpp>

namespace S3DCustomGUI {

using DropDownMenuDrawEvent = std::function<int(int arg_y)>;
class DropDownMenu
{
public:
	DropDownMenu();
public:
	int Draw(s3d::Vec2 arg_position, int arg_width);

	bool IsOpen() const
	{
		return this->is_open;
	}
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
	void SetDrawEvent(DropDownMenuDrawEvent arg_draw_event)
	{
		this->draw_event = arg_draw_event;
	}

private:
	bool					is_open = false;

	s3d::String				top_label = U"";
	s3d::Vec2				position = Vec2(0,0);
	int						width = 100;
	DropDownMenuDrawEvent	draw_event = [](int arg_y){return arg_y;};
};

}

