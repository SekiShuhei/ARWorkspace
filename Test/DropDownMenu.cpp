#include "DropDownMenu.hpp"

namespace ARWorkspace {
namespace CustomGUI {

DropDownMenu::DropDownMenu(DropDownMenuDrawEvent arg_draw_event) :
	draw_event(arg_draw_event)
{

}
bool DropDownMenu::Draw(s3d::Vec2 arg_position)
{
	if (! this->is_open)
	{
		// �g�b�v���x���{�^���̂ݕ\������.

		return true;
	}
	// ��ԏ�ɂ�����{�^���z�u.

	bool result = this->draw_event();

	// ����{�^���\��.

	return result;
}


}
}