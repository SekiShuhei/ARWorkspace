
#include <Siv3D.hpp>
//#include <tuple>
#include "KeyCommand.hpp"

ARWorkspace::KeyCommand::KeyCommand(ARVirtualScreen& arg_model) :
	model(arg_model)
{

}

void ARWorkspace::KeyCommand::Update()
{
	
	auto offset = this->getPositionOffset();
	if (! offset)
	{
		return;
	}
	if (! KeyShift.pressed())
	{
		this->model.SetCaptureRegionPosition(
			this->model.GetCaptureRegion().x + std::get<0>(offset.value()),
			this->model.GetCaptureRegion().y + std::get<1>(offset.value()));
	} else {
		this->model.SetCaptureRegionSize(
			this->model.GetCaptureRegion().w + std::get<0>(offset.value()),
			this->model.GetCaptureRegion().h + std::get<1>(offset.value()));
	}
}

std::optional<std::tuple<int, int>> ARWorkspace::KeyCommand::getPositionOffset() const
{
	int offset_x = 0;
	int offset_y = 0;
	if (KeyUp.down() || KeyUp.pressedDuration().count() > 1.0)
	{
		return std::tuple<int, int>(0, -1);
	}
	else if (KeyDown.down() || KeyDown.pressedDuration().count() > 1.0)
	{
		return std::tuple<int, int>(0, 1);
	}
	else if (KeyLeft.down() || KeyLeft.pressedDuration().count() > 1.0)
	{
		return std::tuple<int, int>(-1, 0);
	}
	else if (KeyRight.down() || KeyRight.pressedDuration().count() > 1.0)
	{
		return std::tuple<int, int>(1, 0);
	}
	else {
		return std::nullopt;
	}
}
