#pragma once

#include <atlcoll.h>
#include "SensorManagerDefine.hpp"

namespace WinSensor {
class SensorMapCollction{

public:
	// with ISensor* AddRef() and SetEventSink().
	bool Set(SENSOR_ID arg_key, ISensor* arg_p_sensor, ISensorEvents* arg_p_sensor_events);
	// with ISensor* Release() and SetEventSink().
	bool Remove(SENSOR_ID arg_key);
	//bool Remove(ISensor*);
	bool RemoveAll();
private:
	CAtlMap<SENSOR_ID, ISensor*>	inner_map;
};

}

