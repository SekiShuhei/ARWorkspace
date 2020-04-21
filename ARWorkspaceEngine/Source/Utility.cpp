

#include "Utility.hpp"

namespace ARWorkspace {

const s3d::String Utility::GetExecFilePath()
{
	s3d::String result_path = U"";
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

	if (::GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
	{
		::_splitpath_s(path, drive, dir, fname, ext);
		result_path = s3d::Unicode::Widen(std::string(drive) + std::string(dir));
	}
	return result_path;
}

const s3d::String Utility::GetModulePath()
{
	s3d::String result_path = U"";
	wchar_t path[MAX_PATH];
	if (::GetModuleFileNameW(nullptr, path, MAX_PATH) != 0)
	{
		result_path = s3d::Unicode::FromWString(std::wstring(path));
	}
	return result_path;
}

HWND Utility::GetMainWindowHandle(s3d::String WindowTitle)
{
	(HWND)s3d::Platform::Windows::Window::GetHWND();

	//return ::FindWindowW(
	//	(LPCWSTR)Utility::GetModulePath().c_str(), 
	//	WindowTitle.toWstr().c_str());
	return 0;
}

}
