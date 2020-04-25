#pragma once

#include <memory>
#include <optional>
#include "SensorManagerDefine.hpp"
#include "SensorEvents.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
class SensorControl final
{
public:
	// New SensorControl Instance with ISensor* AddRef() and SetEventSink().
	static std::optional<SensorControl*> 
		Create(SENSOR_ID sensor_id, ISensor* p_sensor, const SensorRequest& request);
public:
	SensorControl(const SensorControl&)				= delete;
	SensorControl& operator=(const SensorControl&)	= delete;
	SensorControl(const SensorControl&&)				= delete;
	SensorControl& operator=(SensorControl&&)			= delete;
private:
	// with ISensor* AddRef() and SetEventSink().
	SensorControl() noexcept;
public:
	~SensorControl() noexcept;
	// with ISensor* Release() and SetEventSink().
	void Release() noexcept;
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
	bool initialize(SENSOR_ID arg_sensor_id, ISensor* p_sensor, const SensorRequest& request) noexcept;
private:
	ISensor*		p_sensor;
	SENSOR_ID		sensor_id;
	SENSOR_TYPE_ID	type_id;

	std::unique_ptr<SensorEvents> sp_sensor_events;
};

}
