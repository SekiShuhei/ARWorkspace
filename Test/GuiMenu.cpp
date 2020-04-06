#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"

namespace ARWorkspace {

GuiMenu::GuiMenu(std::shared_ptr<ARVirtualScreen> arg_p_model) :
p_model(arg_p_model)
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
			// Žü‚è‚ÉŽlŠpŒ`‘‚­H.
			double w = this->item_width;
			double h = this->item_height;
			double x = this->position.x;
			double y = arg_y;
			if (this->control_capture_region_x.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().x))
			{
				this->p_model->SetCaptureRegionPosition(
					this->control_capture_region_x.GetValue(),
					this->p_model->GetCaptureRegion().y);
			}
			y += h;

			if (this->control_capture_region_y.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().y))
			{
				this->p_model->SetCaptureRegionPosition(
					this->p_model->GetCaptureRegion().x,
					this->control_capture_region_y.GetValue());
			}
			y += h;

			if (this->control_capture_region_width.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().w))
			{
				this->p_model->SetCaptureRegionSize(
					this->control_capture_region_width.GetValue(),
					this->p_model->GetCaptureRegion().h);
			}
			y += h;

			if (this->control_capture_region_height.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().h))
			{
				this->p_model->SetCaptureRegionSize(
					this->p_model->GetCaptureRegion().w,
					this->control_capture_region_height.GetValue());
			}
			y += h;

			if (this->control_capture_scale.Draw(Vec2(x, y), this->p_model->scale, !this->p_model->texture_auto_resize))
			{
				this->p_model->scale = this->control_capture_scale.GetValue();

			}
			y += h;
			return y;
		});

	this->item_width = this->control_capture_region_x.GetTotalWidth();
	this->item_height = 35;
	this->position = s3d::Vec2(s3d::Scene::Size().x - this->item_width, 0);
}

void GuiMenu::Draw()
{
	
	double y = 0;

	y += this->menu_capture_setting.Draw(this->position, this->item_width);

	// add...

}

}