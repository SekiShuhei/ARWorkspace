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
	bool Add(ISensor* p_sensor, const SensorRequest& request);

	// çÌèúó\íË.
	bool Set(SENSOR_ID key, ISensor* p_sensor, ISensorEvents* p_sensor_events);
	// with ISensor* Release() and SetEventSink().
	bool Remove(SENSOR_ID arg_key);
	//bool Remove(ISensor*);
	bool RemoveAll();
private:
	CAtlMap<SENSOR_ID, ISensor*>	inner_map;

	// Ç‹Çæí«â¡ÇæÇØ.

	std::map<SENSOR_ID, std::unique_ptr<SensorInfo>> info_map;
};

}

