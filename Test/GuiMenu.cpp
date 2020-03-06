

#include "GuiMenu.hpp"

namespace ARWorkspace {

	GuiMenu::GuiMenu(ARVirtualScreen& arg_model) :
	model(arg_model)
{


}

void GuiMenu::Draw()
{
	
	//if (! this->pull_down_flag)
	//{
	//	// 開くボタン.
	//	//...
	//	this->pull_down_flag = true;
	//	return;
	//}

	// キャプチャ領域指定.
	double x = s3d::Scene::Size().x - 300;
	double y = 0;
	double h = 30;

	if (SimpleGUI::Slider(U"R {:.2f}"_fmt(this->model.GetCaptureRegion().x),
		this->model.GetCaptureRegion().x, 0, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"R {:.2f}"_fmt(this->model.GetCaptureRegion().y),
		this->model.GetCaptureRegion().y, 0, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"R {:.2f}"_fmt(this->model.GetCaptureRegion().w),
		this->model.GetCaptureRegion().w, 200, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;
	if (SimpleGUI::Slider(U"R {:.2f}"_fmt(this->model.GetCaptureRegion().h),
		this->model.GetCaptureRegion().h, 200, 2000, Vec2(x, y), 100, 200))
	{
		this->model.CaptureRegionUpdate();
		this->model.CaptureSizeUpdate();
	}
	y += h;




}

}