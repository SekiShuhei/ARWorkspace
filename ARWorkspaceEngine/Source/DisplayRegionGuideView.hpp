#pragma once

class DisplayRegion;

namespace ARWorkspace {

class DisplayRegionGuideView
{


public:
	DisplayRegionGuideView() = default;
	DisplayRegionGuideView(const DisplayRegion& arg_display_region, int arg_border_width);

	bool Draw();

	void Invalidate() const;
    static void Invalidate(const DisplayRegion& display_region, int border_width);

private:

    static bool drawLine(int x, int y, int width, int height, int border_width);
	bool isUpdate() const;

	const DisplayRegion& display_region;
	int		border_width = 10;
	double		x = 0;
	double		y = 0;
	double		width = 200;
	double		height = 200;


};

}