#include "DropDownMenu.hpp"

namespace ARWorkspace {
namespace CustomGUI {

DropDownMenu::DropDownMenu(DropDownMenuDrawEvent arg_draw_event) :
	draw_event(arg_draw_event)
{

}
bool DropDownMenu::Draw()
{
	if (! this->is_open)
	{
		// トップレベルボタンのみ表示する.

		return true;
	}
	// 一番上にも閉じるボタン配置.

	bool result = this->draw_event();

	// 閉じるボタン表示.

	return result;
}


}
}