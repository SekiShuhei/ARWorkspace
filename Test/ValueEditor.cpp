#include "ValueEditor.hpp"

CustomGUI::ValueEditor::ValueEditor()
{


}

CustomGUI::ValueEditor::~ValueEditor()
{
}

bool CustomGUI::ValueEditor::Draw(s3d::Vec2 arg_position, double arg_new_value)
{
	this->ValueModelUpdate(arg_new_value);

	bool result = false;
	this->position = arg_position;
	// 表示位置の調整方法？？.


	//if (SimpleGUI::Slider(U"X {:.2f}"_fmt(this->model.GetCaptureRegion().x),
	//	this->model.GetCaptureRegion().x, 0, 2000, Vec2(x, y), text_width, slider_width))
	

	if (SimpleGUI::TextBox(text_edit_state, this->position, 300))
	{

		// テキストボックス編集時の挙動　文字列のパース.
		
		


		// 値が変更されたらtrueを返す.
		result = this->valueChange(0.0);
	}

	return result;
}

void CustomGUI::ValueEditor::ValueModelUpdate(double arg_new_value)
{
	this->value_model = arg_new_value;
	this->text_edit_state.text = s3d::Format(this->value_model);

}

bool CustomGUI::ValueEditor::valueChange(double arg_new_value)
{
	double last_value = this->value_model;
	this->value_model = arg_new_value;
	this->value_change_event(arg_new_value, last_value);
	
	return true;
}



