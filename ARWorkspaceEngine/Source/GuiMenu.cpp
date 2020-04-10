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
	this->control_capture_region_width.SetRange(50.0, 2000.0, 100.0);
	this->control_capture_region_width.SetWidth(label_width, slider_width, textbox_width);
	this->control_capture_region_height.SetLabel(U"height");
	this->control_capture_region_height.SetRange(50.0, 2000.0, 100.0);
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
			if (this->control_capture_region_x.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().GetX()))
			{
				this->p_model->SetCaptureRegionPosition(
					this->control_capture_region_x.GetValue(),
					this->p_model->GetCaptureRegion().GetY());
			}
			y += h;

			if (this->control_capture_region_y.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().GetY()))
			{
				this->p_model->SetCaptureRegionPosition(
					this->p_model->GetCaptureRegion().GetX(),
					this->control_capture_region_y.GetValue());
			}
			y += h;

			if (this->control_capture_region_width.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().GetWidth()))
			{
				this->p_model->SetCaptureRegionSize(
					this->control_capture_region_width.GetValue(),
					this->p_model->GetCaptureRegion().GetHeight());
			}
			y += h;

			if (this->control_capture_region_height.Draw(Vec2(x, y), this->p_model->GetCaptureRegion().GetHeight()))
			{
				this->p_model->SetCaptureRegionSize(
					this->p_model->GetCaptureRegion().GetWidth(),
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

	this->menu_key_guide.SetLabel(U"KeyGuide");
	this->menu_key_guide.SetDrawEvent([this](int arg_y)
		{
			
			s3d::SimpleGUI::Headline(U"Shift+¨:CapturePoint", Vec2(this->position.x, arg_y), this->item_width);
			arg_y += this->item_height;
			s3d::SimpleGUI::Headline(U"Alt+¨:CaptureSize", Vec2(this->position.x, arg_y), this->item_width);
			arg_y += this->item_height;
			return arg_y;
		});


	this->item_width = this->control_capture_region_x.GetTotalWidth();
	this->item_height = 35;
	this->position = s3d::Vec2(s3d::Scene::Size().x - this->item_width, 0);
}

void GuiMenu::Draw()
{
	this->position = s3d::Vec2(s3d::Scene::Size().x - this->item_width, 0);
	double y = 0;

	if (! this->menu_key_guide.IsOpen())
	{
		y += this->menu_capture_setting.Draw(s3d::Vec2(this->position.x, y), this->item_width);
		y += 5;
	}
	if (! this->menu_capture_setting.IsOpen())
	{
		y += this->menu_key_guide.Draw(s3d::Vec2(this->position.x, y), this->item_width);
	}
	// add...

}

}