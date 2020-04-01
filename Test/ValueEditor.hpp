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

private:
	bool valueChange(double arg_new_value);

private:

	double							value_model = 0.0;
	double							default_value = 0.0;
	double							max_value = 100.0;
	double							min_value = 0.0;
	ValueEditorValueChangedEvent	value_change_event = [](auto, auto){};
	s3d::Vec2						position;
	s3d::TextEditState				text_edit_state;

};
}