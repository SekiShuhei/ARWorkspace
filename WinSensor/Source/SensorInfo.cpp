#include "SensorInfo.hpp"

namespace WinSensor {

// �������G���[�̏ꍇ�͐����ł��Ȃ��悤�ɂ���ׂ�.
// static Create���`����H�H.
SensorInfo::SensorInfo(SENSOR_ID arg_sensor_id, ISensor* p_sensor, const SensorRequest& request)
{
	// Create SensorEvents.
	this->sp_sensor_events = std::make_unique<SensorEvents>(request.callback_func);

	// ISensor.
	HRESULT hr;
	hr = p_sensor->SetEventSink(this->sp_sensor_events.get());
	if (SUCCEEDED(hr))
	{
		p_sensor->AddRef();
	}
	this->sensor_id = arg_sensor_id;
	this->type_id = request.type_id;
}

SensorInfo::~SensorInfo()
{
	// ISensor.
	p_sensor->SetEventSink(NULL);
	p_sensor->Release();
}

}
