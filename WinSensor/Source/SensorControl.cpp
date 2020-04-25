#include "SensorControl.hpp"

namespace WinSensor {
std::optional<SensorControl*> SensorControl::Create(SENSOR_ID sensor_id, ISensor* p_sensor, const SensorRequest& request)
{
	auto p = new SensorControl();
	if (! p->initialize(sensor_id, p_sensor, request))
	{
		return std::nullopt;
	}
	return p;
}
SensorControl::SensorControl() noexcept :
	p_sensor(nullptr),
	sensor_id(GUID_NULL),
	type_id(GUID_NULL)
{
	
}

SensorControl::~SensorControl()
{
}

void SensorControl::Release() noexcept
{
	// ISensor.
	this->p_sensor->SetEventSink(NULL);
	this->p_sensor->Release();
}

bool SensorControl::initialize(SENSOR_ID arg_sensor_id, ISensor* arg_p_sensor, const SensorRequest& request) noexcept
{
	// Create SensorEvents.
	this->sp_sensor_events = std::make_unique<SensorEvents>(
			request.callback_data_updated_func,
			request.callback_sensor_leaved_func);

	// ISensor.
	HRESULT hr;
	hr = arg_p_sensor->SetEventSink(this->sp_sensor_events.get());
	if (SUCCEEDED(hr))
	{
		this->p_sensor = arg_p_sensor;
		this->p_sensor->AddRef();
		this->sensor_id = arg_sensor_id;
		this->type_id = request.type_id;

		return true;
	}
	return false;

}

}
