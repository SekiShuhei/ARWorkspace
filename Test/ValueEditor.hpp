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

	bool Draw(s3d::Vec2	arg_position, double arg_new_value, bool arg_enable = true);
	void ValueModelUpdate(double arg_new_value);

	double GetValue() const
	{
		return this->value_model;
	}
	int GetTotalWidth() const
	{
		return this->caption_width + this->slider_width + this->textbox_width;
	}
	void SetRange(double arg_min_value, double arg_max_value, double arg_default_value)
	{
		this->min_value = arg_min_value;
		this->max_value = arg_max_value;
		this->default_value = arg_default_value;
	}
	void SetWidth(int arg_caption_width, int arg_slider_width, int arg_textbox_width)
	{
		this->caption_width = arg_caption_width;
		this->slider_width = arg_slider_width;
		this->textbox_width = arg_textbox_width;
	}
	void SetLabel(s3d::String arg_label_string)
	{
		this->label = arg_label_string;
	}

private:
	bool valueChange(double arg_new_value);

	s3d::String valueToString()
	{
		return U"{:.0f}"_fmt(this->value_model);
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