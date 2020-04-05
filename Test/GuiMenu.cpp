

#include "GuiMenu.hpp"

namespace ARWorkspace {

GuiMenu::GuiMenu(ARVirtualScreen& arg_model) :
model(arg_model)
{

	int label_width = 80;
	int slider_width = 200;
	int textbox_width = 100;

	this->control_capture_region_x.SetLabel(U"X");
	this->control_capture_region_x.SetRange(0.0, 4000.0, 0.0);
	this->control_capture_region_x.SetWidth(label_width, slider_width, textbox_width);
	this->control_capture_region_y.SetLabel(U"Y");
	this->control_capture_region_y.SetRange(0.0, 2000.0, 0.0);
	this->control_capture_region_y.SetWidth(label_width, slider_width, textbox_width);
	this->control_capture_region_width.SetLabel(U"width");
	this->control_capture_region_width.SetRange(0.0, 2000.0, 100.0);
	this->control_capture_region_width.SetWidth(label_width, slider_width, textbox_width);
	this->control_capture_region_height.SetLabel(U"height");
	this->control_capture_region_height.SetRange(0.0, 2000.0, 100.0);
	this->control_capture_region_height.SetWidth(label_width, slider_width, textbox_width);
	this->control_capture_scale.SetLabel(U"scale");
	this->control_capture_scale.SetRange(0.0, 20.0, 2.0);
	this->control_capture_scale.SetWidth(label_width, slider_width, textbox_width);

	this->menu_capture_setting.SetLabel(U"CaptureSetting");
	this->menu_capture_setting.SetDrawEvent([this](int arg_y)
		{
			// kari.
			double w = this->control_capture_region_x.GetTotalWidth();
			double h = 30;
			double x = s3d::Scene::Size().x - w;
			double y = arg_y;
			if (this->control_capture_region_x.Draw(Vec2(x, y), this->model.GetCaptureRegion().x))
			{
				this->model.SetCaptureRegionPosition(
					this->control_capture_region_x.GetValue(),
					this->model.GetCaptureRegion().y);
			}
			y += h;

			if (this->control_capture_region_y.Draw(Vec2(x, y), this->model.GetCaptureRegion().y))
			{
				this->model.SetCaptureRegionPosition(
					this->model.GetCaptureRegion().x,
					this->control_capture_region_y.GetValue());
			}
			y += h;

			if (this->control_capture_region_width.Draw(Vec2(x, y), this->model.GetCaptureRegion().w))
			{
				this->model.SetCaptureRegionSize(
					this->control_capture_region_width.GetValue(),
					this->model.GetCaptureRegion().h);
			}
			y += h;

			if (this->control_capture_region_height.Draw(Vec2(x, y), this->model.GetCaptureRegion().h))
			{
				this->model.SetCaptureRegionSize(
					this->model.GetCaptureRegion().w,
					this->control_capture_region_height.GetValue());
			}
			y += h;

			if (this->control_capture_scale.Draw(Vec2(x, y), this->model.scale, !this->model.texture_auto_resize))
			{
				this->model.scale = this->control_capture_scale.GetValue();

			}
			y += h;
			return y;
		});
}

void GuiMenu::Draw()
{
	int text_width = 150;
	int slider_width = 200;

	double w = this->control_capture_region_x.GetTotalWidth();
	double h = 30;
	double x = s3d::Scene::Size().x - w;
	double y = 0;

	/// test

	y += this->menu_capture_setting.Draw(Vec2(x, y), w);

	/// test/
	
	if (! this->pull_down_flag)
	{
		if (s3d::SimpleGUI::Button(U"��CaptureSetting", Vec2(x,y), w))
		{
			this->pull_down_flag = true;
		}
		return;
	}
	
	{
		
		if (this->control_capture_region_x.Draw(Vec2(x, y), this->model.GetCaptureRegion().x))
		{
			this->model.SetCaptureRegionPosition(
				this->control_capture_region_x.GetValue(),
				this->model.GetCaptureRegion().y);
		}
		y += h;

		if (this->control_capture_region_y.Draw(Vec2(x, y), this->model.GetCaptureRegion().y))
		{
			this->model.SetCaptureRegionPosition(
				this->model.GetCaptureRegion().x,
				this->control_capture_region_y.GetValue());
		}
		y += h;

		if (this->control_capture_region_width.Draw(Vec2(x, y), this->model.GetCaptureRegion().w))
		{
			this->model.SetCaptureRegionSize(
				this->control_capture_region_width.GetValue(),
				this->model.GetCaptureRegion().h);
		}
		y += h;

		if (this->control_capture_region_height.Draw(Vec2(x, y), this->model.GetCaptureRegion().h))
		{
			this->model.SetCaptureRegionSize(
				this->model.GetCaptureRegion().w,
				this->control_capture_region_height.GetValue());
		}
		y += h;

		if (this->control_capture_scale.Draw(Vec2(x, y), this->model.scale, !this->model.texture_auto_resize))
		{
			this->model.scale = this->control_capture_scale.GetValue();

		}
		y += h;
	}

	if (s3d::SimpleGUI::Button(U"��Close", Vec2(x, y), w))
	{
		this->pull_down_flag = false;
	}

}

}