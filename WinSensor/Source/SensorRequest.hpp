#pragma once

#include <string>
#include "SensorManagerDefine.hpp"

namespace WinSensor {
struct SensorRequest
{
public:
	SensorRequest() : type_id(GUID_NULL)
	{

	};
	std::wstring				caption;
	SENSOR_TYPE_ID				type_id;
	std::vector<std::wstring>	vid_list;

	SensorEventCallbackFunction	callback_func = 
		[](ISensor*, ISensorDataReport*){ return false; };
};

}
