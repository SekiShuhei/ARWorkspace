#pragma once

#include <Siv3D.hpp>

# define  NOMINMAX
# define  WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ARWorkspace {
namespace Utility
{
	const s3d::String GetExecFilePath();
	const s3d::String GetModulePath();

	HWND GetMainWindowHandle(s3d::String);
}

}