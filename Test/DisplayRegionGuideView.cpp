
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#include <windows.h>

#include "DisplayRegion.hpp"

#include "DisplayRegionGuideView.hpp"

namespace ARWorkspace {
DisplayRegionGuideView::DisplayRegionGuideView(const DisplayRegion& arg_display_region, int arg_border_width) :
    display_region(arg_display_region), 
    border_width(arg_border_width)
{

}

bool DisplayRegionGuideView::Draw()
{
    if (this->isUpdate())
    {
        this->Invalidate(this->display_region, this->border_width);

        // マルチディスプレイ環境でGDIが表示されない、位置がずれる等の減少がまれに起こるが謎.
        this->x = this->display_region.x + ::GetSystemMetrics(SM_XVIRTUALSCREEN);
        this->y = this->display_region.y + ::GetSystemMetrics(SM_YVIRTUALSCREEN);
        this->width = this->display_region.w;
        this->height = this->display_region.h;
    }
    this->drawLine((int)this->x, (int)this->y, 
        (int)this->width, (int)this->height, this->border_width);

    return false;
}

void DisplayRegionGuideView::Invalidate() const
{
    DisplayRegionGuideView::Invalidate(this->display_region, this->border_width);
}

void DisplayRegionGuideView::Invalidate(const DisplayRegion& display_region, int border_width)
{
    RECT rect;
    rect.left = display_region.x - border_width;
    rect.top = display_region.y - border_width;
    rect.right = display_region.x + display_region.w + border_width;
    rect.bottom = display_region.y + display_region.h + border_width;
    ::InvalidateRect(NULL, &rect, true);
}

bool DisplayRegionGuideView::drawLine(int x, int y, int width, int height, int border_width)
{
    HDC  desktop_hdc = ::GetDC(NULL);

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
    ::ReleaseDC(NULL, desktop_hdc);

    return true;
}

bool DisplayRegionGuideView::isUpdate() const
{
    return (
        this->x      != this->display_region.x ||
        this->y      != this->display_region.y ||
        this->width  != this->display_region.w ||
        this->height != this->display_region.h);
}

}