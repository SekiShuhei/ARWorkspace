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
	bool Draw();
private:
	bool is_open = false;

	s3d::String		top_label = U"";
	DropDownMenuDrawEvent		draw_event = [](){return false;};
};

}
}

