

#include "GuiMenu.hpp"

namespace ARWorkspace {

	GuiMenu::GuiMenu(ARVirtualScreen& arg_model) :
	model(arg_model)
{


}

void GuiMenu::Draw()
{
	int text_width = 150;
	int slider_width = 200;

	double x = s3d::Scene::Size().x - (text_width + slider_width);
	double y = 0;
	double h = 30;

	
	if (! this->pull_down_flag)
	{
		if (s3d::SimpleGUI::Button(U"¥CaptureSetting", Vec2(x,y), text_width + slider_width))
		{
			this->pull_down_flag = true;
		}
		return;
	}
	
	if (SimpleGUI::Slider(U"X {:.2f}"_fmt(this->model.GetCaptureRegion().x),
		this->model.GetCaptureRegion().x, 0, 2000, Vec2(x, y), text_width, slider_width))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Y {:.2f}"_fmt(this->model.GetCaptureRegion().y),
		this->model.GetCaptureRegion().y, 0, 2000, Vec2(x, y), text_width, slider_width))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Width {:.2f}"_fmt(this->model.GetCaptureRegion().w),
		this->model.GetCaptureRegion().w, 200, 2000, Vec2(x, y), text_width, slider_width))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Height {:.2f}"_fmt(this->model.GetCaptureRegion().h),
		this->model.GetCaptureRegion().h, 200, 2000, Vec2(x, y), text_width, slider_width))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"Scale {:.2f}"_fmt(this->model.scale),
		this->model.scale, 1.0, 10.0, Vec2(x, y), text_width, slider_width, 
		! this->model.texture_auto_resize))
	{
		//...
	}
	y += h;

	//...
	// test.
	{
		if (this->value_editor_width.Draw(Vec2(x, y), this->model.GetCaptureRegion().x))
		{
			this->model.GetCaptureRegion().x = this->value_editor_width.GetValue();
			
		}
		y += h;
	}

	if (s3d::SimpleGUI::Button(U"£Close", Vec2(x, y), text_width + slider_width))
	{
		this->pull_down_flag = false;
	}

}

}