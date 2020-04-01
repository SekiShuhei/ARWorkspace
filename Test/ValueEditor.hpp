#pragma once


#include <Siv3D.hpp>

namespace CustomGUI
{

using ValueEditorValueChangedEvent = std::function<void(double arg_new_value, double arg_last_value)>;
class ValueEditor
{
public:
	ValueEditor() = default;
	ValueEditor(double arg_value_model, ValueEditorValueChangedEvent arg_value_change_event);
	~ValueEditor();

	void Draw();

private:

	double							value_model;
	ValueEditorValueChangedEvent	value_change_event;
	s3d::Vec2						position;
	s3d::TextEditState				text_edit_state;

};
}