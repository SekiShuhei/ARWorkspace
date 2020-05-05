
#include <algorithm>
#include "DisplayInfoUtility.hpp"

namespace ARWorkspace {
bool DisplayInfoUtility::EnumDisplayInfo() noexcept
{
	::EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)enumDisplayCallback, (LPARAM)this);

	return false;
}

std::optional<const DisplayInfo> DisplayInfoUtility::GetPrimaryDisplayInfo() const noexcept
{
	
	auto it = std::find_if(this->info_list.begin(), this->info_list.end(), 
		[](const std::shared_ptr<DisplayInfo> info)
		{
			return info->primary_monitor;
		});
	if (it == this->info_list.end())
	{
		return std::nullopt;
	}
	return **it;
}

std::optional<const DisplayInfo> DisplayInfoUtility::GetSubDisplayInfo() const noexcept
{
	auto it = std::find_if(this->info_list.begin(), this->info_list.end(),
		[](const std::shared_ptr <DisplayInfo> info)
		{
			return (! info->primary_monitor);
		});
	if (it == this->info_list.end())
	{
		return std::nullopt;
	}
	return **it;
}

std::optional<const DisplayInfo> DisplayInfoUtility::FindDisplayInfo(const std::wstring& arg_device_name) const noexcept
{
	auto it = std::find_if(this->info_list.begin(), this->info_list.end(),
		[arg_device_name](const std::shared_ptr <DisplayInfo> info)
		{
			return (info->device_name.find(arg_device_name) != std::wstring::npos);
		});
	if (it == this->info_list.end())
	{
		return std::nullopt;
	}
	return **it;
}

ScreenRegion DisplayInfoUtility::RectToScreenRegion(const RECT& rect) noexcept
{
	ScreenRegion region;
	region.SetX(rect.left);
	region.SetY(rect.top);
	region.SetWidth((double)rect.right - (double)rect.left);
	region.SetHeight((double)rect.bottom - (double)rect.top);

	return region;
}

BOOL CALLBACK DisplayInfoUtility::enumDisplayCallback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lpMonitor, LPARAM dwDate)
{
	MONITORINFOEX monitorInfo;

	monitorInfo.cbSize = sizeof(monitorInfo);
	::GetMonitorInfo(hMonitor, &monitorInfo);

	auto p_info = std::make_shared<DisplayInfo>();
	p_info->monitor = DisplayInfoUtility::RectToScreenRegion( monitorInfo.rcMonitor);
	p_info->workspace = DisplayInfoUtility::RectToScreenRegion(monitorInfo.rcWork);
	p_info->device_name = monitorInfo.szDevice;
	p_info->primary_monitor = (monitorInfo.dwFlags == MONITORINFOF_PRIMARY);

	DisplayInfoUtility* p_this = (DisplayInfoUtility*)dwDate;
	p_this->info_list.push_back(p_info);

	return TRUE;
}


}
