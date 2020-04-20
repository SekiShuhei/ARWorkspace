#include "SensorMapCollection.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
bool SensorMapCollction::Set(SENSOR_ID arg_key, ISensor* arg_p_sensor, ISensorEvents* arg_p_sensor_events)
{
	if( arg_p_sensor == nullptr || arg_p_sensor_events == nullptr)
	{
		return false;
	}
	this->Remove(arg_key);

	arg_p_sensor->SetEventSink(arg_p_sensor_events);
	arg_p_sensor->AddRef();
	this->inner_map[arg_key] = arg_p_sensor;

	return true;
}

bool SensorMapCollction::Remove(SENSOR_ID arg_key)
{
	auto p_sensor = this->inner_map[arg_key];
	SENSOR_ID sensor_id = GUID_NULL;
	if (SUCCEEDED(p_sensor->GetID(&sensor_id)))
	{
		p_sensor->SetEventSink(NULL);
		p_sensor->Release();
	}
	this->inner_map.RemoveKey(arg_key);

	return true;
}

bool SensorMapCollction::RemoveAll()
{
	POSITION pos = this->inner_map.GetStartPosition();
	while (NULL != pos)
	{
		auto key = this->inner_map.GetNextKey(pos);
		this->Remove(key);
	}
	return false;
}

}
