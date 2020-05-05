#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <windows.h>
#include "ScreenRegion.hpp"

namespace ARWorkspace {
struct DisplayInfo
{
	ScreenRegion	monitor;
	ScreenRegion	workspace;
	bool		primary_monitor = false;
	std::wstring	device_name = L"";
};

class DisplayInfoUtility
{

public:

public:
	bool EnumDisplayInfo() noexcept;
	std::optional<const DisplayInfo> GetPrimaryDisplayInfo() const noexcept;
	std::optional<const DisplayInfo> GetSubDisplayInfo() const noexcept;
	std::optional<const DisplayInfo> FindDisplayInfo(const std::wstring& device_name) const noexcept;

public:
	static ScreenRegion RectToScreenRegion(const RECT& rect) noexcept;

private:
	static BOOL CALLBACK enumDisplayCallback(HMONITOR hMon, HDC hdcMon, LPRECT lpMon, LPARAM dwDate);

private:
	std::vector<std::shared_ptr<DisplayInfo>> info_list;
};

}