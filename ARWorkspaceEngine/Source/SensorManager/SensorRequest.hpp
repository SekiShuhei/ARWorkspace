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

	SENSOR_TYPE_ID				type_id;
	std::vector<std::wstring>	vid_list;

	// callback function...
};

}
