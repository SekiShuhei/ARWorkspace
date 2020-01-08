#include <Windows.h>

#include "Utilty.hpp"

std::string Utilty::GetModulePath()
{
	// 実行ファイルのパス
	std::string modulePath = "";
	// ドライブ名、ディレクトリ名、ファイル名、拡張子
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

	// 実行ファイルのファイルパスを取得
	if (::GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		// ファイルパスを分割
		::_splitpath_s(path, drive, dir, fname, ext);
		// ドライブとディレクトリ名を結合して実行ファイルパスとする
		modulePath = std::string(drive) + std::string(dir);
	}

	return modulePath;
}
