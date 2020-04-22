#pragma once

#include <map>
#include <memory>
#include <atlcoll.h>
#include "SensorManagerDefine.hpp"
#include "SensorRequest.hpp"
#include "SensorEvents.hpp"
#include "SensorInfo.hpp"
namespace WinSensor {
class SensorInfoMapManager final
{

public:
	SensorInfoMapManager();
	~SensorInfoMapManager();
	// with ISensor* AddRef() and SetEventSink().
	bool Add(ISensor* p_sensor, const SensorRequest& request) noexcept;

	// �폜�\��.
	//bool Set(SENSOR_ID key, ISensor* p_sensor, ISensorEvents* p_sensor_events);
	// with ISensor* Release() and SetEventSink().
	bool Remove(SENSOR_ID arg_sensor_id) noexcept;
	
	bool RemoveAll();
private:
	//CAtlMap<SENSOR_ID, ISensor*>	inner_map;

private:
	// ���Q��public�ɂ���.
	bool removeSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept;
	bool removeSensorInfoFromType(SENSOR_TYPE_ID arg_sensor_id) noexcept;
	inline void deleteSensorInfo(SensorInfo* p_info) noexcept
	{
		p_info->Release();
		delete p_info;
	}

private:
	// �܂��ǉ�����.

	std::vector<SensorInfo*> info_map;
};

}

