#pragma once

#include "ARVirtualScreen.hpp"

namespace ARWorkspace {

class KeyCommand
{
public:
	KeyCommand(ARVirtualScreen& arg_model);
public:
	void Update();

private:

	ARVirtualScreen&	model; //�ق�Ƃ���shared_ptr�ɂ��ׂ��� �Ƃ肠����

	// F1 + �\���L�[�ŃL���v�`���J�n�n�_�̈ړ�.
	// F2 + �\���L�[�ŃL���v�`���I���n�_�̈ړ�.
	// F3 + �\���L�[�ŃX�P�[���ύX�H�i�����j
	// ����K�C�h�\���̌���.
};

}

