#include "ValueEditor.hpp"

CustomGUI::ValueEditor::ValueEditor(double arg_value_model, ValueEditorValueChangedEvent arg_value_change_event) :
	value_model( arg_value_model),
	value_change_event(arg_value_change_event)
{


}

void CustomGUI::ValueEditor::Draw()
{
	// 表示位置の調整方法？？.


	// テキストボックス編集時の挙動　文字列のパース.


	// 値変更時にラムダ式を実行.
}



