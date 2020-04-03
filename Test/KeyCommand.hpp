#pragma once

#include "ARVirtualScreen.hpp"

namespace ARWorkspace {

class KeyCommand
{
public:
	KeyCommand(ARVirtualScreen& arg_model);
public:
	void Update();

private:

	ARVirtualScreen&	model; //ほんとうはshared_ptrにすべきか とりあえず

	// F1 + 十字キーでキャプチャ開始地点の移動.
	// F2 + 十字キーでキャプチャ終了地点の移動.
	// F3 + 十字キーでスケール変更？（検討）
	// 操作ガイド表示の検討.
};

}

