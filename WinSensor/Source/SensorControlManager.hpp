#pragma once

#include <map>
#include <memory>
#include <atlcoll.h>
#include "SensorManagerDefine.hpp"
#include "SensorRequest.hpp"
#include "SensorEvents.hpp"
#include "SensorControl.hpp"
namespace WinSensor {
class SensorControlManager final
{
public:
	SensorControlManager();
	~SensorControlManager();
	// with ISensor* AddRef() and SetEventSink().
	HRESULT Add(ISensor* p_sensor, SensorRequest& request) noexcept;

	// with ISensor* Release() and SetEventSink().
	bool RemoveSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept;
	bool RemoveSensorInfoFromType(SENSOR_TYPE_ID arg_sensor_id) noexcept;
	bool RemoveAll();

private:
	inline void deleteSensorInfo(SensorControl* p_info) noexcept
	{
		p_info->Release();
		delete p_info;
	}

private:
	std::vector<SensorControl*> p_control_list;
};

}

