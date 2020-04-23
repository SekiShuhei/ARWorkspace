#include "SensorInfo.hpp"

namespace WinSensor {
std::optional<SensorInfo*> SensorInfo::Create(SENSOR_ID sensor_id, ISensor* p_sensor, const SensorRequest& request)
{
	auto p = new SensorInfo();
	if (! p->initialize(sensor_id, p_sensor, request))
	{
		return std::nullopt;
	}
	return p;
}
SensorInfo::SensorInfo() noexcept :
	p_sensor(nullptr),
	sensor_id(GUID_NULL),
	type_id(GUID_NULL)
{
	
}

SensorInfo::~SensorInfo()
{
}

void SensorInfo::Release() noexcept
{
	// ISensor.
	this->p_sensor->SetEventSink(NULL);
	this->p_sensor->Release();
}

bool SensorInfo::initialize(SENSOR_ID arg_sensor_id, ISensor* arg_p_sensor, const SensorRequest& request) noexcept
{
	// Create SensorEvents.
	this->sp_sensor_events = std::make_unique<SensorEvents>(request.callback_func);

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
