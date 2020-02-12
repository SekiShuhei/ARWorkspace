#pragma once

#include "DisplayRegion.hpp"


static class DisplayRegionGuideView
{
	static bool Draw(const DisplayRegion& display_region)
	{
		return DisplayRegionGuideView::Draw(
			(int)display_region.x, (int)display_region.y,
			(int)display_region.w, (int)display_region.h);
	}

	static bool Draw(int x, int y, int width, int height)
	{
		//...

		return true;
	}

};