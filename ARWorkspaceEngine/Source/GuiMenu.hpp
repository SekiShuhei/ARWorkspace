#pragma once

#include "ARWorkspaceEngine.hpp"

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
	
	S3DCustomGUI::DropDownMenu  menu_capture_setting;
	S3DCustomGUI::DropDownMenu  menu_key_guide;

	S3DCustomGUI::ValueEditor	control_capture_region_x;
	S3DCustomGUI::ValueEditor	control_capture_region_y;
	S3DCustomGUI::ValueEditor	control_capture_region_width;
	S3DCustomGUI::ValueEditor	control_capture_region_height;
	S3DCustomGUI::ValueEditor	control_capture_scale;
};

}

