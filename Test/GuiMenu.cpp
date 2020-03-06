

#include "GuiMenu.hpp"

namespace ARWorkspace {

	GuiMenu::GuiMenu(ARVirtualScreen& arg_model) :
	model(arg_model)
{


}

void GuiMenu::Draw()
{
	double x = s3d::Scene::Size().x - 300;
	double y = 0;
	double h = 30;

	
	if (! this->pull_down_flag)
	{
		if (s3d::SimpleGUI::Button(U"¥CaptureSetting", Vec2(x,y)))
		{
			this->pull_down_flag = true;
		}
		return;
	}

	if (SimpleGUI::Slider(U"X {:.2f}"_fmt(this->model.GetCaptureRegion().x),
		this->model.GetCaptureRegion().x, 0, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Y {:.2f}"_fmt(this->model.GetCaptureRegion().y),
		this->model.GetCaptureRegion().y, 0, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Width {:.2f}"_fmt(this->model.GetCaptureRegion().w),
		this->model.GetCaptureRegion().w, 200, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Height {:.2f}"_fmt(this->model.GetCaptureRegion().h),
		this->model.GetCaptureRegion().h, 200, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;

	//...

	if (s3d::SimpleGUI::Button(U"£Close", Vec2(x, y)))
	{
		this->pull_down_flag = false;
	}

}

}