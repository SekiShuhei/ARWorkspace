#pragma once

#include <string>
#include "SensorManagerDefine.hpp"

namespace WinSensor {

//TODO:
// リクエスト結果enum class（エラー情報含む）の定義.
// リクエスト結果を構造体内に保持する
	
// 接続情報のstructを新設.
// 共通で扱われるセンサ情報は別途構造体として独立させる.

struct SensorRequest
{
public:
	SensorRequest() : type_id(GUID_NULL)
	{

	};
	std::wstring				name;
	SENSOR_TYPE_ID				type_id;
	std::vector<std::wstring>	vid_list;

	SensorEventCallback_OnDataUpdatedFunction	callback_data_updated_func =
		[](ISensor*, ISensorDataReport*){ return S_OK; };

	SensorEventCallback_OnLeavedFunction callback_sensor_leaved_func = 
		[](REFSENSOR_ID){ return S_OK; };

};

}
