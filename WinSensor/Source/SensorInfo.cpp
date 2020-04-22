#include "SensorInfo.hpp"

namespace WinSensor {

// 初期化エラーの場合は生成できないようにするべき.
// static Createを定義する？？.
SensorInfo::SensorInfo(SENSOR_ID arg_sensor_id, ISensor* arg_p_sensor, const SensorRequest& request) noexcept
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

		// return true.
	}
	// return false.

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

}
