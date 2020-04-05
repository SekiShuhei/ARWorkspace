#pragma once

#include <Siv3D.hpp>

#include "ARVirtualScreen.hpp"
#include "ValueEditor.hpp"
#include "DropDownMenu.hpp"

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
	GuiMenu() = default;
	GuiMenu(ARVirtualScreen&);
	void Draw();

private:

	ARVirtualScreen&	model;

	GuiMenuAlignment	alignment = GuiMenuAlignment::RightTop;
	bool pull_down_flag = false;
	int text_size = 20;

	CustomGUI::DropDownMenu  menu_capture_setting;

	CustomGUI::ValueEditor	control_capture_region_x;
	CustomGUI::ValueEditor	control_capture_region_y;
	CustomGUI::ValueEditor	control_capture_region_width;
	CustomGUI::ValueEditor	control_capture_region_height;
	CustomGUI::ValueEditor	control_capture_scale;
};

}

