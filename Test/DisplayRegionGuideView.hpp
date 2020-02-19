#pragma once

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#include <windows.h>

#include "DisplayRegion.hpp"

namespace ARWorkspace {

static class DisplayRegionGuideView
{
public:
	static bool Draw(const DisplayRegion& display_region, int border_width)
	{
		return DisplayRegionGuideView::Draw(
			(int)display_region.x, (int)display_region.y,
			(int)display_region.w, (int)display_region.h,
            border_width);
	}

	static bool Draw(int x, int y, int width, int height, int border_width)
	{
        DisplayRegionGuideView::drawLine(x, y, width, height, border_width);
		
		return true;
	}

private:

    static bool drawLine(int x, int y, int width, int height, int border_width)
    {
        // チラつきなど見栄えがよくないのでデバッグ用途以外では使えそうにない.
        HWND desktop_hwnd = ::GetDesktopWindow();
        HDC  desktop_hdc = ::GetDC(desktop_hwnd);

        int bw = border_width / 2;
        HPEN   hpen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 127));
        HBRUSH hbrush = ::CreateSolidBrush(RGB(0, 255, 127));
        HPEN   oldpen = (HPEN)::SelectObject(desktop_hdc, (HGDIOBJ*)hpen);
        HBRUSH oldbrush = (HBRUSH)::SelectObject(desktop_hdc, (HGDIOBJ*)hbrush);
        ::Rectangle(desktop_hdc, x - bw, y - bw, x + width + bw, y + bw);
        ::Rectangle(desktop_hdc, x - bw, y - bw, x + bw, y + height + bw);
        ::Rectangle(desktop_hdc, x + width - bw, y - bw, x + width + bw, y + height + bw);
        ::Rectangle(desktop_hdc, x - bw, y + height - bw, x + width + bw, y + height + bw);

        ::SelectObject(desktop_hdc, oldpen);
        ::SelectObject(desktop_hdc, oldbrush);
        ::DeleteObject(hpen);
        ::DeleteObject(hbrush);

        return true;
    }

};

}