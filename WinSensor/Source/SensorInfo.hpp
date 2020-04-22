#pragma once

#include <memory>
#include <optional>
#include "SensorManagerDefine.hpp"
#include "SensorEvents.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
class SensorInfo final
{
public:
	// with ISensor* AddRef() and SetEventSink().
	static std::optional<std::unique_ptr<SensorInfo>> 
		Create(SENSOR_ID arg_sensor_id, ISensor* p_sensor, const SensorRequest& request);
public:
	SensorInfo(const SensorInfo&)				= delete;
	SensorInfo& operator=(const SensorInfo&)	= delete;
	SensorInfo(const SensorInfo&&)				= delete;
	SensorInfo& operator=(SensorInfo&&)			= delete;
public:
//private: //Create関数からのみ生成できるようにすべき.
	SensorInfo() = delete;
	// with ISensor* AddRef() and SetEventSink().
	SensorInfo(SENSOR_ID arg_sensor_id, ISensor* p_sensor, const SensorRequest& request) noexcept;
public:
	// with ISensor* Release() and SetEventSink().
	~SensorInfo();
public:
	SENSOR_ID GetSensorID() const noexcept
	{
		return this->sensor_id;
	}
	SENSOR_TYPE_ID GetSensorTypeID() const noexcept
	{
		return this->type_id;
	}
private:
	ISensor*		p_sensor;
	SENSOR_ID		sensor_id;
	SENSOR_TYPE_ID	type_id;

	std::unique_ptr<SensorEvents> sp_sensor_events;
};

}
