
#include "DisplayInfo.hpp"

namespace ARWorkspace {
bool DisplayInfoUtility::EnumDisplayInfo() noexcept
{
	::EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)enumDisplayCallback, NULL);

	return false;
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

	DisplayInfo	info;
	info.monitor = DisplayInfoUtility::RectToScreenRegion( monitorInfo.rcMonitor);
	info.workspace = DisplayInfoUtility::RectToScreenRegion(monitorInfo.rcWork);
	info.device_name = monitorInfo.szDevice;
	info.primary_monitor = (monitorInfo.dwFlags == MONITORINFOF_PRIMARY);

	return TRUE;
}


}
