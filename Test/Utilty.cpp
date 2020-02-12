#include <Windows.h>

#include "Utilty.hpp"

namespace ARWorkspace {

std::string Utilty::GetModulePath()
{
	// ���s�t�@�C���̃p�X
	std::string modulePath = "";
	// �h���C�u���A�f�B���N�g�����A�t�@�C�����A�g���q
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

	// ���s�t�@�C���̃t�@�C���p�X���擾
	if (::GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		// �t�@�C���p�X�𕪊�
		::_splitpath_s(path, drive, dir, fname, ext);
		// �h���C�u�ƃf�B���N�g�������������Ď��s�t�@�C���p�X�Ƃ���
		modulePath = std::string(drive) + std::string(dir);
	}

	return modulePath;
}

}