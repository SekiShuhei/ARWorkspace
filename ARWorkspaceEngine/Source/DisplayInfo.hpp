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
	// プライマリモニタの情報を得る.
	const std::shared_ptr<DisplayInfo> GetPrimaryDisplayInfo() const noexcept;
	const std::shared_ptr<DisplayInfo> GetSubDisplayInfo(const std::string& device_name) const noexcept;
	// BT-30のデバイス名一致確認して情報を得る.
	const std::shared_ptr<DisplayInfo> FindDisplayInfo(const std::string& device_name) const noexcept;


public:
	static ScreenRegion RectToScreenRegion(const RECT& rect) noexcept;

private:
	static BOOL CALLBACK enumDisplayCallback(HMONITOR hMon, HDC hdcMon, LPRECT lpMon, LPARAM dwDate);

private:
	std::vector<std::shared_ptr<DisplayInfo>> info_list;
};

}