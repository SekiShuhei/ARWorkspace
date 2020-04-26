#pragma once

#include <string>
#include "SensorManagerDefine.hpp"

namespace WinSensor {
enum class SensorRequestState
{
	Default = 0,
	Requesting,
	SensorTypeError,
	SensorNotFound,
	DeviceNotFound,
	RequestError,
	Connected,
	Disconnected,
};
	
enum class SensorRequestTargetState
{
	Generic = 0,
	Priority,
};

struct SensorRequest
{
public:
	SensorRequest() : type_id(GUID_NULL)
	{

	};



public:
	std::wstring				name;
	SensorRequestState			state			= SensorRequestState::Default;
	SensorRequestTargetState	target_state	= SensorRequestTargetState::Generic;
	SENSOR_TYPE_ID				type_id;
	std::vector<std::wstring>	vid_list;

	SensorEventCallback_OnDataUpdatedFunction	callback_data_updated_func =
		[](ISensor*, ISensorDataReport*){ return S_OK; };

	SensorEventCallback_OnLeavedFunction callback_sensor_leaved_func = 
		[](REFSENSOR_ID){ return S_OK; };

};

}
