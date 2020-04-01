#pragma once


#include <Siv3D.hpp>

namespace CustomGUI
{

using ValueEditorValueChangedEvent = std::function<void(double arg_new_value, double arg_last_value)>;
class ValueEditor
{
public:
	ValueEditor();
	//ValueEditor(ValueEditorValueChangedEvent arg_value_change_event);
	~ValueEditor();

	bool Draw(s3d::Vec2	arg_position, double arg_new_value);
	void ValueModelUpdate(double arg_new_value);

	double GetValue() const
	{
		return this->value_model;
	}
	int GetTotalWidth() const
	{
		return this->caption_width + this->slider_width + this->textbox_width;
	}

private:
	bool valueChange(double arg_new_value);

	s3d::String valueToString()
	{
		return U"{:.2f}"_fmt(this->value_model);
	}

private:

	double							value_model = 0.0;
	double							last_update_value = -1.0;
	double							default_value = 0.0;
	double							max_value = 100.0;
	double							min_value = 0.0;
	ValueEditorValueChangedEvent	value_change_event = [](auto, auto){};
	s3d::TextEditState				text_edit_state;

	s3d::Vec2						position;
	s3d::String						label = U"value";
	int								caption_width = 80;
	int								slider_width = 200;
	int								textbox_width = 100;
};
}