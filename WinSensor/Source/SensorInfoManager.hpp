#pragma once

#include <map>
#include <memory>
#include <atlcoll.h>
#include "SensorManagerDefine.hpp"
#include "SensorRequest.hpp"
#include "SensorEvents.hpp"
#include "SensorInfo.hpp"
namespace WinSensor {
class SensorInfoManager final
{
public:
	SensorInfoManager();
	~SensorInfoManager();
	// with ISensor* AddRef() and SetEventSink().
	HRESULT Add(ISensor* p_sensor, const SensorRequest& request) noexcept;

	// with ISensor* Release() and SetEventSink().
	bool RemoveSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept;
	bool RemoveSensorInfoFromType(SENSOR_TYPE_ID arg_sensor_id) noexcept;
	bool RemoveAll();

private:
	inline void deleteSensorInfo(SensorInfo* p_info) noexcept
	{
		p_info->Release();
		delete p_info;
	}

private:
	std::vector<SensorInfo*> p_info_list;
};

}

