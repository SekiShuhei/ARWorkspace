#pragma once

#include <Siv3D.hpp>

#include "ARVirtualScreen.hpp"
#include "ValueEditor.hpp"

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

	CustomGUI::ValueEditor	value_editor_width;
};

}

