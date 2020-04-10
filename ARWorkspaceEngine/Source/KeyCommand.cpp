
#include <Siv3D.hpp>
#include "ARVirtualScreen.hpp"
#include "KeyCommand.hpp"

ARWorkspace::KeyCommand::KeyCommand(std::shared_ptr<ARVirtualScreen> arg_p_model) :
	p_model(arg_p_model)
{

}

void ARWorkspace::KeyCommand::Update()
{
	
	auto offset = this->getPositionOffset();
	if (! offset)
	{
		return;
	}
	if (KeyShift.pressed())
	{
		this->p_model->SetCaptureRegionPosition(
			this->p_model->GetCaptureRegion().GetX() + std::get<0>(offset.value()),
			this->p_model->GetCaptureRegion().GetY() + std::get<1>(offset.value()));
	}
	if (KeyAlt.pressed())
	{
		this->p_model->SetCaptureRegionSize(
				this->p_model->GetCaptureRegion().GetWidth() + std::get<0>(offset.value()),
				this->p_model->GetCaptureRegion().GetHeight() + std::get<1>(offset.value()));
	}
}

std::optional<std::tuple<int, int>> ARWorkspace::KeyCommand::getPositionOffset() const
{
	int offset_x = 0;
	int offset_y = 0;
	if (KeyUp.down() || KeyUp.pressedDuration().count() > 1.0)
	{
		offset_y = -1;
	}
	if (KeyDown.down() || KeyDown.pressedDuration().count() > 1.0)
	{
		offset_y = 1;
	}
	if (KeyLeft.down() || KeyLeft.pressedDuration().count() > 1.0)
	{
		offset_x = -1;
	}
	if (KeyRight.down() || KeyRight.pressedDuration().count() > 1.0)
	{
		offset_x = 1;
	}
	if (offset_x == 0 && offset_y == 0)
	{
		return std::nullopt;
	} else {
		return std::tuple<int,int>(offset_x, offset_y);
	}
}
