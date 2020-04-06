#pragma once

#include <Siv3D.hpp>

#include "ValueEditor.hpp"
#include "DropDownMenu.hpp"

class ARVirtualScreen;

namespace ARWorkspace {
enum class GuiMenuAlignment
{
	LeftTop,
	RightTop,
	LeftBottom,
	RightBottom,
	Center,
};

class GuiMenu
{
public:
	GuiMenu() = delete;
	GuiMenu(std::shared_ptr<ARVirtualScreen>);
	void Draw();

private:

	std::shared_ptr<ARVirtualScreen>	p_model;

	GuiMenuAlignment	alignment = GuiMenuAlignment::RightTop;
	bool pull_down_flag = false;
	s3d::Vec2		position;
	int				item_width;
	int				item_height;
	
	CustomGUI::DropDownMenu  menu_capture_setting;

	CustomGUI::ValueEditor	control_capture_region_x;
	CustomGUI::ValueEditor	control_capture_region_y;
	CustomGUI::ValueEditor	control_capture_region_width;
	CustomGUI::ValueEditor	control_capture_region_height;
	CustomGUI::ValueEditor	control_capture_scale;
};

}

