#include "ValueEditor.hpp"

namespace ARWorkspace {
namespace CustomGUI {

ValueEditor::ValueEditor()
{


}

ValueEditor::~ValueEditor()
{
}

bool ValueEditor::Draw(s3d::Vec2 arg_position, double arg_new_value, bool arg_enable)
{
	this->ValueModelUpdate(arg_new_value);

	bool value_change_flag = false;
	s3d::String last_textbox_string = this->text_edit_state.text;
	this->position = arg_position;
	int x = arg_position.x;
	// .

	if (SimpleGUI::Slider(this->label, this->value_model, this->min_value, this->max_value, 
		Vec2(x, this->position.y), this->caption_width, this->slider_width,
		arg_enable))
	{
		value_change_flag = true;
		this->text_edit_state.text = this->valueToString();

	}
	x += this->caption_width + this->slider_width;
	if (SimpleGUI::TextBox(text_edit_state, Vec2(x, this->position.y), 
		this->textbox_width, 4, arg_enable))
	{
		value_change_flag = true;
		try
		{
			auto new_value = Parse<double>(text_edit_state.text);
			if (new_value > this->max_value)
			{
				new_value = this->max_value;
			}
			if (new_value < this->min_value)
			{
				new_value = this->min_value;
			}
			this->valueChange(new_value);
		}
		catch(const s3d::ParseError& error)
		{
			text_edit_state.text = last_textbox_string;
		}
	}
	

	return value_change_flag;
}

void ValueEditor::ValueModelUpdate(double arg_new_value)
{
	if (this->last_update_value != arg_new_value)
	{
		this->last_update_value = arg_new_value;
		this->value_model = arg_new_value;
		this->text_edit_state.text = this->valueToString();
	}
}

bool ValueEditor::valueChange(double arg_new_value)
{
	
	double last_value = this->value_model;
	this->value_model = arg_new_value;
	this->value_change_event(arg_new_value, last_value);
	
	return true;
}

}
}


