#define WIN32_LEAN_AND_MEAN
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
    // �Ȃ�Ƃ����ăp�t�H�[�}���X���グ��.
    if (this->isUpdate())
    {
        int screen_x = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
        int screen_y = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
        int screen_w = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int screen_h = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

        if (this->display_region.GetX() + screen_x > screen_w ||
            this->display_region.GetY() + screen_y > screen_h)
        {
            return false;
        }

        this->Invalidate(this->display_region, this->border_width);

        // TODO:�X�P�[�����O���.
        this->x = this->display_region.GetX() + screen_x;
        this->y = this->display_region.GetY() + screen_y;
        this->width = this->display_region.GetWidth();
        this->height = this->display_region.GetHeight();
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
    rect.left = display_region.GetX() - border_width;
    rect.top = display_region.GetY() - border_width;
    rect.right = display_region.GetX() + display_region.GetWidth() + border_width;
    rect.bottom = display_region.GetY() + display_region.GetHeight() + border_width;
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

    // TODO:���z�X�N���[���̗̈���ɃV�������N������.
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
        this->x      != this->display_region.GetX() ||
        this->y      != this->display_region.GetY() ||
        this->width  != this->display_region.GetWidth() ||
        this->height != this->display_region.GetHeight());
}

}