#include "SensorInfoMap.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
SensorMapCollction::SensorMapCollction()
{

}
SensorMapCollction::~SensorMapCollction()
{
	this->RemoveAll();
}
bool SensorMapCollction::Set(SENSOR_ID key, ISensor* p_sensor, ISensorEvents* p_sensor_events)
{
	if( p_sensor == nullptr || p_sensor_events == nullptr)
	{
		return false;
	}
	this->Remove(key);

	p_sensor->SetEventSink(p_sensor_events);
	p_sensor->AddRef();
	this->inner_map[key] = p_sensor;

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
	return true;
}

}
