#pragma once

#include <atlcoll.h>
#include "SensorManagerDefine.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
class SensorInfoMap final
{

public:
	SensorInfoMap();
	~SensorInfoMap();
	// with ISensor* AddRef() and SetEventSink().
	bool Set(SENSOR_ID key, ISensor* p_sensor, ISensorEvents* p_sensor_events);
	// with ISensor* Release() and SetEventSink().
	bool Remove(SENSOR_ID arg_key);
	//bool Remove(ISensor*);
	bool RemoveAll();
private:
	CAtlMap<SENSOR_ID, ISensor*>	inner_map;
};

}

